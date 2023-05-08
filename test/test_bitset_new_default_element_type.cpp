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
  template <size_t Active_Bits>
  ETL_CONSTEXPR14 etl::bitset<Active_Bits> generate_shift_left_bitset(ull value, size_t shift)
  {
    etl::bitset<Active_Bits> data(value);

    if (shift != 0U)
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

    if (shift != 0U)
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

    if (shift != 0U)
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
    TEST(test_default_constructor)
    {
      ETL_CONSTEXPR14 std::bitset<60> compare;
      ETL_CONSTEXPR14 etl::bitset<60> data;

      CHECK_EQUAL(compare.size(),  data.size());
      CHECK_EQUAL(compare.count(), data.count());

      for (size_t i = 0UL; i < data.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data.test(i));
      }
    }

    //*************************************************************************
    TEST(test_default_constructor_multi_element)
    {
      etl::bitset<14, int8_t>  data8;
      etl::bitset<30, int16_t> data16;
      etl::bitset<62, int32_t> data32;

      CHECK_EQUAL(2U, data8.Number_Of_Elements);
      CHECK_EQUAL(2U, data16.Number_Of_Elements);
      CHECK_EQUAL(2U, data32.Number_Of_Elements);
    }

    //*************************************************************************
    TEST(test_default_constructor_default_element_type_bits)
    {
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<1>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<2>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<3>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<4>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<5>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<6>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<7>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<8>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<9>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<10>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<11>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<12>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<13>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<14>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<15>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<16>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<17>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<18>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<19>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<20>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<21>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<22>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<23>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<24>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<25>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<26>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<27>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<28>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<29>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<30>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<31>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<32>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<33>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<34>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<35>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<36>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<37>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<38>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<39>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<40>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<41>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<42>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<43>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<44>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<45>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<46>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<47>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<48>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<49>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<50>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<51>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<52>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<53>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<54>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<55>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<56>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<57>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<58>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<59>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<60>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<61>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<62>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<63>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset<64>::element_type>::bits);
    }
     
    //*************************************************************************
    TEST(test_default_constructor_default_element_type_number_of_elements)
    {
      CHECK_EQUAL(1U, etl::bitset<1>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset<2>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset<3>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset<4>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset<5>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset<6>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset<7>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset<8>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<9>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<10>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<11>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<12>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<13>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<14>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<15>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset<16>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<17>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<18>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<19>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<20>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<21>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<22>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<23>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset<24>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<25>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<26>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<27>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<28>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<29>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<30>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<31>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset<32>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<33>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<34>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<35>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<36>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<37>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<38>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<39>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset<40>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<41>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<42>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<43>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<44>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<45>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<46>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<47>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset<48>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<49>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<50>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<51>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<52>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<53>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<54>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<55>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset<56>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<57>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<58>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<59>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<60>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<61>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<62>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<63>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset<64>::Number_Of_Elements);
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int8_t_element_types_bits)
    {
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<1, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<2, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<3, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<4, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<5, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<6, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<7, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<8, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<9, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<10, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<11, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<12, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<13, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<14, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<15, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<16, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<17, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<18, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<19, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<20, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<21, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<22, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<23, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<24, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<25, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<26, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<27, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<28, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<29, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<30, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<31, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<32, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<33, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<34, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<35, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<36, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<37, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<38, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<39, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<40, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<41, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<42, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<43, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<44, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<45, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<46, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<47, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<48, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<49, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<50, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<51, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<52, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<53, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<54, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<55, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<56, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<57, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<58, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<59, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<60, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<61, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<62, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<63, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset<64, int8_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int8_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset<1, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<2, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<3, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<4, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<5, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<6, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<7, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<8, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<9, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<10, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<11, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<12, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<13, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<14, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<15, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<16, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<17, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<18, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<19, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<20, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<21, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<22, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<23, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<24, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<25, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<26, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<27, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<28, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<29, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<30, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<31, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<32, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<33, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<34, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<35, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<36, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<37, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<38, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<39, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset<40, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<41, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<42, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<43, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<44, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<45, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<46, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<47, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset<48, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<49, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<50, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<51, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<52, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<53, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<54, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<55, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset<56, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<57, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<58, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<59, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<60, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<61, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<62, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<63, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset<64, int8_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int16_t_element_types_bits)
    {
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<1, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<2, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<3, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<4, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<5, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<6, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<7, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<8, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<9, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<10, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<11, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<12, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<13, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<14, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<15, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<16, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<17, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<18, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<19, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<20, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<21, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<22, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<23, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<24, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<25, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<26, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<27, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<28, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<29, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<30, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<31, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<32, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<33, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<34, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<35, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<36, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<37, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<38, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<39, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<40, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<41, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<42, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<43, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<44, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<45, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<46, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<47, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<48, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<49, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<50, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<51, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<52, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<53, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<54, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<55, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<56, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<57, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<58, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<59, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<60, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<61, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<62, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<63, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset<64, int16_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int16_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset<1, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<2, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<3, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<4, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<5, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<6, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<7, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<8, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<9, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<10, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<11, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<12, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<13, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<14, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<15, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<16, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<17, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<18, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<19, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<20, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<21, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<22, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<23, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<24, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<25, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<26, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<27, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<28, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<29, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<30, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<31, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<32, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<33, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<34, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<35, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<36, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<37, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<38, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<39, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<40, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<41, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<42, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<43, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<44, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<45, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<46, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<47, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset<48, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<49, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<50, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<51, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<52, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<53, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<54, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<55, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<56, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<57, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<58, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<59, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<60, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<61, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<62, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<63, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset<64, int16_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int32_t_element_types_bits)
    {
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<1, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<2, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<3, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<4, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<5, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<6, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<7, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<8, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<9, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<10, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<11, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<12, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<13, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<14, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<15, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<16, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<17, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<18, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<19, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<20, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<21, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<22, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<23, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<24, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<25, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<26, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<27, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<28, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<29, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<30, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<31, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<32, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<33, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<34, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<35, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<36, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<37, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<38, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<39, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<40, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<41, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<42, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<43, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<44, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<45, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<46, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<47, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<48, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<49, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<50, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<51, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<52, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<53, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<54, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<55, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<56, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<57, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<58, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<59, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<60, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<61, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<62, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<63, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset<64, int32_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int32_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset<1, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<2, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<3, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<4, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<5, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<6, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<7, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<8, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<9, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<10, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<11, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<12, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<13, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<14, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<15, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<16, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<17, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<18, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<19, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<20, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<21, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<22, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<23, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<24, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<25, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<26, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<27, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<28, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<29, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<30, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<31, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<32, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<33, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<34, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<35, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<36, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<37, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<38, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<39, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<40, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<41, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<42, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<43, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<44, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<45, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<46, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<47, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<48, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<49, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<50, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<51, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<52, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<53, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<54, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<55, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<56, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<57, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<58, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<59, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<60, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<61, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<62, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<63, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset<64, int32_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int64_t_element_types_bits)
    {
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<1, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<2, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<3, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<4, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<5, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<6, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<7, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<8, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<9, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<10, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<11, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<12, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<13, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<14, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<15, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<16, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<17, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<18, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<19, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<20, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<21, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<22, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<23, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<24, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<25, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<26, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<27, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<28, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<29, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<30, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<31, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<32, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<33, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<34, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<35, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<36, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<37, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<38, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<39, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<40, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<41, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<42, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<43, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<44, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<45, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<46, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<47, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<48, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<49, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<50, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<51, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<52, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<53, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<54, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<55, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<56, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<57, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<58, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<59, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<60, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<61, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<62, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<63, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset<64, int64_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int64_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset<1, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<2, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<3, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<4, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<5, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<6, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<7, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<8, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<9, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<10, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<11, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<12, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<13, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<14, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<15, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<16, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<17, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<18, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<19, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<20, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<21, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<22, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<23, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<24, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<25, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<26, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<27, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<28, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<29, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<30, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<31, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<32, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<33, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<34, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<35, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<36, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<37, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<38, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<39, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<40, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<41, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<42, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<43, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<44, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<45, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<46, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<47, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<48, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<49, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<50, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<51, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<52, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<53, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<54, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<55, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<56, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<57, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<58, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<59, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<60, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<61, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<62, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<63, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset<64, int64_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      ETL_CONSTEXPR14 std::bitset<60> compare(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data(0x123456731234567ULL);

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
      ETL_CONSTEXPR14 std::bitset<60> compare(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data(0x123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data_copy(data);

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
      ETL_CONSTEXPR14 std::bitset<60> compare(0x8765432187654321ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data(0x8765432187654321ULL);

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
      ETL_CONSTEXPR14 etl::bitset<32> data((unsigned long long)etl::integral_limits<uint32_t>::min);
      ETL_CONSTEXPR14 uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_max)
    {
      ETL_CONSTEXPR14 etl::bitset<32> data((unsigned long long)etl::integral_limits<uint32_t>::max);
      ETL_CONSTEXPR14 uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), value);
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
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<uint64_t>::min);
      ETL_CONSTEXPR14 uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      ETL_CONSTEXPR14 etl::bitset<64> data((unsigned long long)etl::integral_limits<uint64_t>::max);
      ETL_CONSTEXPR14 uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), value);
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
    TEST(test_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data(0xFFFFFFFFFFFFFFFULL);

      compare.reset();
      data.reset();

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
    TEST(test_position_reset)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data(0xFFFFFFFFFFFFFFFULL);

      compare.reset(1);
      compare.reset(3);
      compare.reset(7);
      compare.reset(13);

      data.reset(1);
      data.reset(3);
      data.reset(7);
      data.reset(13);

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
    TEST(test_index_operator_read)
    {
      ETL_CONSTEXPR14 std::bitset<60> compare(0x3123456731234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data(0x3123456731234567ULL);

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
    TEST(test_index_operator_write)
    {
      etl::bitset<60> data;

      data[1U] = true;
      data[3U] = true;
      data[7U] = true;
      data[13U] = true;

      CHECK(data[1U]);
      CHECK(data[3U]);
      CHECK(data[7U]);
      CHECK(data[13U]);
    }

    //*************************************************************************
    TEST(test_any)
    {
      ETL_CONSTEXPR14 etl::bitset<60> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data3(ull(0x0000010001000100));
      ETL_CONSTEXPR14 etl::bitset<60> data4(ull(0x0FFFFFFFFFFFFFFF));

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
      ETL_CONSTEXPR14 etl::bitset<60> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data3(ull(0x0000010001000100));
      ETL_CONSTEXPR14 etl::bitset<60> data4(ull(0x0FFFFFFFFFFFFFFF));

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
      ETL_CONSTEXPR14 etl::bitset<60> data1(ull(0x0000000000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data2(ull(0x0000010000000000));
      ETL_CONSTEXPR14 etl::bitset<60> data3(ull(0x0000010001000100));
      ETL_CONSTEXPR14 etl::bitset<60> data4(ull(0x0FFFFFFFFFFFFFFF));

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
    TEST(test_flip_position)
    {
      std::bitset<60> compare;
      etl::bitset<60> data;

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
      std::bitset<60> compare;
      etl::bitset<60> data;

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
      std::bitset<60> compare;
      etl::bitset<60> data;

      bool bc = ~compare[3];
      bool bd = ~data[3];
      CHECK_EQUAL(bc, bd);
    }

    //*************************************************************************
    void test_assignment_operator_helper(etl::bitset<60>& from, etl::bitset<60>& to)
    {
      to = from;
    }

    TEST(test_assignment_operator)
    {
      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data1(0xFFFFFFFFFFFFFFFULL);
      etl::bitset<60> data2;

      test_assignment_operator_helper(data1, data2);

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<60> data1(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data2(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data3;

      ETL_CONSTEXPR14 bool equal     =  (data1 == data2);
      ETL_CONSTEXPR14 bool not_equal = !(data1 == data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<60> data1(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data2(0x123456781234567ULL);
      ETL_CONSTEXPR14 etl::bitset<60> data3;

      ETL_CONSTEXPR14 bool equal = !(data1 != data2);
      ETL_CONSTEXPR14 bool not_equal = (data1 != data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_shift_left_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<60> shift1(0x2468ACF0ULL);
      ETL_CONSTEXPR14 etl::bitset<60> shift2(0x48D159E0ULL);
      ETL_CONSTEXPR14 etl::bitset<60> shift11(0x91A2B3C000ULL);

      CHECK(generate_shift_left_bitset<60>(0x12345678ULL, 1U) == shift1);
      CHECK(generate_shift_left_bitset<60>(0x12345678ULL, 2U) == shift2);
      CHECK(generate_shift_left_bitset<60>(0x12345678ULL, 11U) == shift11);
    }

    //*************************************************************************
    TEST(test_shift_left_operator_overflow)
    {
      ETL_CONSTEXPR14 etl::bitset<31> data(generate_shift_left_bitset<31>(0x7FFFFFFFULL, 1U));
      ETL_CONSTEXPR14 etl::bitset<31> shifted(0x7FFFFFFEUL);

      CHECK(data == shifted);
    }

    //*************************************************************************
    TEST(test_shift_left_copy_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<60> shift8(0x1234567800ULL);
      CHECK_EQUAL_HEX(shift8.value<uint64_t>(), generate_shift_left_bitset_copy<60>(0x12345678UL, 8U).value<uint64_t>());

      ETL_CONSTEXPR14 etl::bitset<60> shift16(0x123456780000ULL);
      CHECK_EQUAL_HEX(shift16.value<uint64_t>(), generate_shift_left_bitset_copy<60>(0x12345678UL, 16U).value<uint64_t>());

      ETL_CONSTEXPR14 etl::bitset<60> shift24(0x12345678000000ULL);
      CHECK_EQUAL_HEX(shift24.value<uint64_t>(), generate_shift_left_bitset_copy<60>(0x12345678UL, 24U).value<uint64_t>());

      ETL_CONSTEXPR14 etl::bitset<60> shift60(0x0000000000000000ULL);
      CHECK_EQUAL_HEX(shift60.value<uint64_t>(), generate_shift_left_bitset_copy<60>(0x12345678UL, 60U).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_operator_all_shifts_partial_size)
    {
      uint64_t value = 0x0123456789ABCDEFULL;
      uint64_t mask  = 0x0FFFFFFFFFFFFFFFULL;

      CHECK_EQUAL_HEX((value << 0U) & mask, (generate_shift_left_bitset<60>(value, 0U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 1U) & mask, (generate_shift_left_bitset<60>(value, 1U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 2U) & mask, (generate_shift_left_bitset<60>(value, 2U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 3U) & mask, (generate_shift_left_bitset<60>(value, 3U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 4U) & mask, (generate_shift_left_bitset<60>(value, 4U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 5U) & mask, (generate_shift_left_bitset<60>(value, 5U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 6U) & mask, (generate_shift_left_bitset<60>(value, 6U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 7U) & mask, (generate_shift_left_bitset<60>(value, 7U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 8U) & mask, (generate_shift_left_bitset<60>(value, 8U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 9U) & mask, (generate_shift_left_bitset<60>(value, 9U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 10U) & mask, (generate_shift_left_bitset<60>(value, 10U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 11U) & mask, (generate_shift_left_bitset<60>(value, 11U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 12U) & mask, (generate_shift_left_bitset<60>(value, 12U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 13U) & mask, (generate_shift_left_bitset<60>(value, 13U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 14U) & mask, (generate_shift_left_bitset<60>(value, 14U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 15U) & mask, (generate_shift_left_bitset<60>(value, 15U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 16U) & mask, (generate_shift_left_bitset<60>(value, 16U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 17U) & mask, (generate_shift_left_bitset<60>(value, 17U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 18U) & mask, (generate_shift_left_bitset<60>(value, 18U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 19U) & mask, (generate_shift_left_bitset<60>(value, 19U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 20U) & mask, (generate_shift_left_bitset<60>(value, 20U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 21U) & mask, (generate_shift_left_bitset<60>(value, 21U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 22U) & mask, (generate_shift_left_bitset<60>(value, 22U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 23U) & mask, (generate_shift_left_bitset<60>(value, 23U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 24U) & mask, (generate_shift_left_bitset<60>(value, 24U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 25U) & mask, (generate_shift_left_bitset<60>(value, 25U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 26U) & mask, (generate_shift_left_bitset<60>(value, 26U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 27U) & mask, (generate_shift_left_bitset<60>(value, 27U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 28U) & mask, (generate_shift_left_bitset<60>(value, 28U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 29U) & mask, (generate_shift_left_bitset<60>(value, 29U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 30U) & mask, (generate_shift_left_bitset<60>(value, 30U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 31U) & mask, (generate_shift_left_bitset<60>(value, 31U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 32U) & mask, (generate_shift_left_bitset<60>(value, 32U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 33U) & mask, (generate_shift_left_bitset<60>(value, 33U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 34U) & mask, (generate_shift_left_bitset<60>(value, 34U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 35U) & mask, (generate_shift_left_bitset<60>(value, 35U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 36U) & mask, (generate_shift_left_bitset<60>(value, 36U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 37U) & mask, (generate_shift_left_bitset<60>(value, 37U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 38U) & mask, (generate_shift_left_bitset<60>(value, 38U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 39U) & mask, (generate_shift_left_bitset<60>(value, 39U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 40U) & mask, (generate_shift_left_bitset<60>(value, 40U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 41U) & mask, (generate_shift_left_bitset<60>(value, 41U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 42U) & mask, (generate_shift_left_bitset<60>(value, 42U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 43U) & mask, (generate_shift_left_bitset<60>(value, 43U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 44U) & mask, (generate_shift_left_bitset<60>(value, 44U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 45U) & mask, (generate_shift_left_bitset<60>(value, 45U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 46U) & mask, (generate_shift_left_bitset<60>(value, 46U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 47U) & mask, (generate_shift_left_bitset<60>(value, 47U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 48U) & mask, (generate_shift_left_bitset<60>(value, 48U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 49U) & mask, (generate_shift_left_bitset<60>(value, 49U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 50U) & mask, (generate_shift_left_bitset<60>(value, 50U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 51U) & mask, (generate_shift_left_bitset<60>(value, 51U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 52U) & mask, (generate_shift_left_bitset<60>(value, 52U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 53U) & mask, (generate_shift_left_bitset<60>(value, 53U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 54U) & mask, (generate_shift_left_bitset<60>(value, 54U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 55U) & mask, (generate_shift_left_bitset<60>(value, 55U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 56U) & mask, (generate_shift_left_bitset<60>(value, 56U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 57U) & mask, (generate_shift_left_bitset<60>(value, 57U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 58U) & mask, (generate_shift_left_bitset<60>(value, 58U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 59U) & mask, (generate_shift_left_bitset<60>(value, 59U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 60U) & mask, (generate_shift_left_bitset<60>(value, 60U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 61U) & mask, (generate_shift_left_bitset<60>(value, 61U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 62U) & mask, (generate_shift_left_bitset<60>(value, 62U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value << 63U) & mask, (generate_shift_left_bitset<60>(value, 63U).value<uint64_t>()));
      CHECK_EQUAL_HEX(0ULL,                  (generate_shift_left_bitset<60>(value, 64U).value<uint64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<60> shift1(0x91A2B3CULL);
      ETL_CONSTEXPR14 etl::bitset<60> shift2(0x48D159EULL);
      ETL_CONSTEXPR14 etl::bitset<60> shift11(0x2468AULL);

      CHECK(generate_shift_right_bitset<60>(0x12345678ULL, 1U) == shift1);
      CHECK(generate_shift_right_bitset<60>(0x12345678ULL, 2U) == shift2);
      CHECK(generate_shift_right_bitset<60>(0x12345678ULL, 11U) == shift11);
    }

    //*************************************************************************
    TEST(test_shift_right_copy_operator)
    {
      ETL_CONSTEXPR14 etl::bitset<60> shift8(0x123456ULL);
      CHECK_EQUAL_HEX(shift8.value<uint64_t>(), generate_shift_right_bitset_copy<60>(0x12345678UL, 8U).value<uint64_t>());

      ETL_CONSTEXPR14 etl::bitset<60> shift16(0x1234ULL);
      CHECK_EQUAL_HEX(shift16.value<uint64_t>(), generate_shift_right_bitset_copy<60>(0x12345678UL, 16U).value<uint64_t>());

      ETL_CONSTEXPR14 etl::bitset<60> shift24(0x12ULL);
      CHECK_EQUAL_HEX(shift24.value<uint64_t>(), generate_shift_right_bitset_copy<60>(0x12345678UL, 24U).value<uint64_t>());

      ETL_CONSTEXPR14 etl::bitset<60> shift60(0x00ULL);
      CHECK_EQUAL_HEX(shift60.value<uint64_t>(), generate_shift_right_bitset_copy<60>(0x12345678UL, 60U).value<uint64_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_full_size)
    {
      uint64_t value = 0x0123456789ABCDEFULL;

      CHECK_EQUAL_HEX((value >> 0U), (generate_shift_right_bitset<64>(value, 0U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 1U), (generate_shift_right_bitset<64>(value, 1U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 2U), (generate_shift_right_bitset<64>(value, 2U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 3U), (generate_shift_right_bitset<64>(value, 3U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 4U), (generate_shift_right_bitset<64>(value, 4U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 5U), (generate_shift_right_bitset<64>(value, 5U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 6U), (generate_shift_right_bitset<64>(value, 6U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 7U), (generate_shift_right_bitset<64>(value, 7U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 8U), (generate_shift_right_bitset<64>(value, 8U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 9U), (generate_shift_right_bitset<64>(value, 9U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 10U), (generate_shift_right_bitset<64>(value, 10U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 11U), (generate_shift_right_bitset<64>(value, 11U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 12U), (generate_shift_right_bitset<64>(value, 12U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 13U), (generate_shift_right_bitset<64>(value, 13U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 14U), (generate_shift_right_bitset<64>(value, 14U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 15U), (generate_shift_right_bitset<64>(value, 15U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 16U), (generate_shift_right_bitset<64>(value, 16U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 17U), (generate_shift_right_bitset<64>(value, 17U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 18U), (generate_shift_right_bitset<64>(value, 18U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 19U), (generate_shift_right_bitset<64>(value, 19U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 20U), (generate_shift_right_bitset<64>(value, 20U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 21U), (generate_shift_right_bitset<64>(value, 21U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 22U), (generate_shift_right_bitset<64>(value, 22U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 23U), (generate_shift_right_bitset<64>(value, 23U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 24U), (generate_shift_right_bitset<64>(value, 24U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 25U), (generate_shift_right_bitset<64>(value, 25U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 26U), (generate_shift_right_bitset<64>(value, 26U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 27U), (generate_shift_right_bitset<64>(value, 27U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 28U), (generate_shift_right_bitset<64>(value, 28U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 29U), (generate_shift_right_bitset<64>(value, 29U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 30U), (generate_shift_right_bitset<64>(value, 30U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 31U), (generate_shift_right_bitset<64>(value, 31U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 32U), (generate_shift_right_bitset<64>(value, 32U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 33U), (generate_shift_right_bitset<64>(value, 33U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 34U), (generate_shift_right_bitset<64>(value, 34U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 35U), (generate_shift_right_bitset<64>(value, 35U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 36U), (generate_shift_right_bitset<64>(value, 36U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 37U), (generate_shift_right_bitset<64>(value, 37U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 38U), (generate_shift_right_bitset<64>(value, 38U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 39U), (generate_shift_right_bitset<64>(value, 39U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 40U), (generate_shift_right_bitset<64>(value, 40U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 41U), (generate_shift_right_bitset<64>(value, 41U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 42U), (generate_shift_right_bitset<64>(value, 42U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 43U), (generate_shift_right_bitset<64>(value, 43U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 44U), (generate_shift_right_bitset<64>(value, 44U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 45U), (generate_shift_right_bitset<64>(value, 45U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 46U), (generate_shift_right_bitset<64>(value, 46U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 47U), (generate_shift_right_bitset<64>(value, 47U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 48U), (generate_shift_right_bitset<64>(value, 48U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 49U), (generate_shift_right_bitset<64>(value, 49U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 50U), (generate_shift_right_bitset<64>(value, 50U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 51U), (generate_shift_right_bitset<64>(value, 51U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 52U), (generate_shift_right_bitset<64>(value, 52U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 53U), (generate_shift_right_bitset<64>(value, 53U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 54U), (generate_shift_right_bitset<64>(value, 54U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 55U), (generate_shift_right_bitset<64>(value, 55U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 56U), (generate_shift_right_bitset<64>(value, 56U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 57U), (generate_shift_right_bitset<64>(value, 57U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 58U), (generate_shift_right_bitset<64>(value, 58U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 59U), (generate_shift_right_bitset<64>(value, 59U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 60U), (generate_shift_right_bitset<64>(value, 60U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 61U), (generate_shift_right_bitset<64>(value, 61U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 62U), (generate_shift_right_bitset<64>(value, 62U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 63U), (generate_shift_right_bitset<64>(value, 63U).value<uint64_t>()));
      CHECK_EQUAL_HEX(0ULL, (generate_shift_right_bitset<64>(value, 64U).value<uint64_t>()));
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_partial_size)
    {
      uint64_t value = 0x0123456789ABCDEFULL;
      uint64_t mask  = 0x0FFFFFFFFFFFFFFFULL;

      CHECK_EQUAL_HEX((value >> 0U) & mask, (generate_shift_right_bitset<60>(value, 0U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 1U) & mask, (generate_shift_right_bitset<60>(value, 1U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 2U) & mask, (generate_shift_right_bitset<60>(value, 2U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 3U) & mask, (generate_shift_right_bitset<60>(value, 3U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 4U) & mask, (generate_shift_right_bitset<60>(value, 4U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 5U) & mask, (generate_shift_right_bitset<60>(value, 5U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 6U) & mask, (generate_shift_right_bitset<60>(value, 6U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 7U) & mask, (generate_shift_right_bitset<60>(value, 7U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 8U) & mask, (generate_shift_right_bitset<60>(value, 8U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 9U) & mask, (generate_shift_right_bitset<60>(value, 9U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 10U) & mask, (generate_shift_right_bitset<60>(value, 10U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 11U) & mask, (generate_shift_right_bitset<60>(value, 11U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 12U) & mask, (generate_shift_right_bitset<60>(value, 12U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 13U) & mask, (generate_shift_right_bitset<60>(value, 13U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 14U) & mask, (generate_shift_right_bitset<60>(value, 14U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 15U) & mask, (generate_shift_right_bitset<60>(value, 15U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 16U) & mask, (generate_shift_right_bitset<60>(value, 16U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 17U) & mask, (generate_shift_right_bitset<60>(value, 17U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 18U) & mask, (generate_shift_right_bitset<60>(value, 18U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 19U) & mask, (generate_shift_right_bitset<60>(value, 19U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 20U) & mask, (generate_shift_right_bitset<60>(value, 20U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 21U) & mask, (generate_shift_right_bitset<60>(value, 21U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 22U) & mask, (generate_shift_right_bitset<60>(value, 22U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 23U) & mask, (generate_shift_right_bitset<60>(value, 23U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 24U) & mask, (generate_shift_right_bitset<60>(value, 24U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 25U) & mask, (generate_shift_right_bitset<60>(value, 25U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 26U) & mask, (generate_shift_right_bitset<60>(value, 26U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 27U) & mask, (generate_shift_right_bitset<60>(value, 27U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 28U) & mask, (generate_shift_right_bitset<60>(value, 28U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 29U) & mask, (generate_shift_right_bitset<60>(value, 29U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 30U) & mask, (generate_shift_right_bitset<60>(value, 30U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 31U) & mask, (generate_shift_right_bitset<60>(value, 31U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 32U) & mask, (generate_shift_right_bitset<60>(value, 32U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 33U) & mask, (generate_shift_right_bitset<60>(value, 33U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 34U) & mask, (generate_shift_right_bitset<60>(value, 34U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 35U) & mask, (generate_shift_right_bitset<60>(value, 35U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 36U) & mask, (generate_shift_right_bitset<60>(value, 36U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 37U) & mask, (generate_shift_right_bitset<60>(value, 37U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 38U) & mask, (generate_shift_right_bitset<60>(value, 38U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 39U) & mask, (generate_shift_right_bitset<60>(value, 39U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 40U) & mask, (generate_shift_right_bitset<60>(value, 40U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 41U) & mask, (generate_shift_right_bitset<60>(value, 41U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 42U) & mask, (generate_shift_right_bitset<60>(value, 42U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 43U) & mask, (generate_shift_right_bitset<60>(value, 43U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 44U) & mask, (generate_shift_right_bitset<60>(value, 44U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 45U) & mask, (generate_shift_right_bitset<60>(value, 45U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 46U) & mask, (generate_shift_right_bitset<60>(value, 46U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 47U) & mask, (generate_shift_right_bitset<60>(value, 47U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 48U) & mask, (generate_shift_right_bitset<60>(value, 48U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 49U) & mask, (generate_shift_right_bitset<60>(value, 49U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 50U) & mask, (generate_shift_right_bitset<60>(value, 50U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 51U) & mask, (generate_shift_right_bitset<60>(value, 51U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 52U) & mask, (generate_shift_right_bitset<60>(value, 52U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 53U) & mask, (generate_shift_right_bitset<60>(value, 53U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 54U) & mask, (generate_shift_right_bitset<60>(value, 54U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 55U) & mask, (generate_shift_right_bitset<60>(value, 55U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 56U) & mask, (generate_shift_right_bitset<60>(value, 56U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 57U) & mask, (generate_shift_right_bitset<60>(value, 57U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 58U) & mask, (generate_shift_right_bitset<60>(value, 58U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 59U) & mask, (generate_shift_right_bitset<60>(value, 59U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 60U) & mask, (generate_shift_right_bitset<60>(value, 60U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 61U) & mask, (generate_shift_right_bitset<60>(value, 61U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 62U) & mask, (generate_shift_right_bitset<60>(value, 62U).value<uint64_t>()));
      CHECK_EQUAL_HEX((value >> 63U) & mask, (generate_shift_right_bitset<60>(value, 63U).value<uint64_t>()));
      CHECK_EQUAL_HEX(0ULL, (generate_shift_right_bitset<60>(value, 64U).value<uint64_t>()));
    }

    //*************************************************************************
    TEST(test_and_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3(0x12345678UL & 0x23456789UL);
      etl::bitset<60> data4(data1 & data2);
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_and_equals_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3(0x12345678UL & 0x23456789UL);

      etl::bitset<60> data4(data1);
      data4 &= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_or_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data4(0x12345678UL | 0x23456789UL);

      etl::bitset<60> data3(data1 | data2);
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_or_equals_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3(0x12345678UL | 0x23456789UL);

      etl::bitset<60> data4(data1);
      data4 |= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_xor_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data4(0x12345678UL ^ 0x23456789UL);

      etl::bitset<60> data3(data1 ^ data2);
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_xor_equals_operator)
    {
      etl::bitset<60> data1(0x12345678UL);
      etl::bitset<60> data2(0x23456789UL);
      etl::bitset<60> data3(0x12345678UL ^ 0x23456789UL);

      etl::bitset<60> data4(data1);
      data4 ^= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      ETL_CONSTEXPR14 etl::bitset<9> bs1(ull(0x0000));
      ETL_CONSTEXPR14 size_t bs1fff = bs1.find_first(false);
      ETL_CONSTEXPR14 size_t bs1fft = bs1.find_first(true);
      CHECK_EQUAL(0U, bs1fff);
      CHECK_EQUAL(etl::bitset<>::npos, bs1fft);

      ETL_CONSTEXPR14 etl::bitset<9> bs2(ull(0x01FF));
      ETL_CONSTEXPR14 size_t bs2fff = bs2.find_first(false);
      ETL_CONSTEXPR14 size_t bs2fft = bs2.find_first(true);
      CHECK_EQUAL(etl::bitset<>::npos, bs2fff);
      CHECK_EQUAL(0U, bs2fft);

      ETL_CONSTEXPR14 etl::bitset<9> bs3(ull(0x0001));
      ETL_CONSTEXPR14 size_t bs3fff = bs3.find_first(false);
      ETL_CONSTEXPR14 size_t bs3fft = bs3.find_first(true);
      CHECK_EQUAL(1U, bs3fff);
      CHECK_EQUAL(0U, bs3fft);

      ETL_CONSTEXPR14 etl::bitset<9> bs4(ull(0x0020));
      ETL_CONSTEXPR14 size_t bs4fff = bs4.find_first(false);
      ETL_CONSTEXPR14 size_t bs4fft = bs4.find_first(true);
      CHECK_EQUAL(0U, bs4fff);
      CHECK_EQUAL(5U, bs4fft);

      ETL_CONSTEXPR14 etl::bitset<9> bs5(ull(0x0021));
      ETL_CONSTEXPR14 size_t bs5fff = bs5.find_first(false);
      ETL_CONSTEXPR14 size_t bs5fft = bs5.find_first(true);
      CHECK_EQUAL(1U, bs5fff);
      CHECK_EQUAL(0U, bs5fft);

      ETL_CONSTEXPR14 etl::bitset<9> bs6(ull(0x000E));
      ETL_CONSTEXPR14 size_t bs6fff = bs6.find_first(false);
      ETL_CONSTEXPR14 size_t bs6fft = bs6.find_first(true);
      CHECK_EQUAL(0U, bs6fff);
      CHECK_EQUAL(1U, bs6fft);

      ETL_CONSTEXPR14 etl::bitset<9> bs7(ull(0x31));
      ETL_CONSTEXPR14 size_t bs7fff = bs7.find_first(false);
      ETL_CONSTEXPR14 size_t bs7fft = bs7.find_first(true);
      CHECK_EQUAL(1U, bs7fff);
      CHECK_EQUAL(0U, bs7fft);
    }
     
    //*************************************************************************
    TEST(test_find_next)
    {
      ETL_CONSTEXPR14 etl::bitset<9> bs1(ull(0x0000));
      ETL_CONSTEXPR14 size_t bs1fnf0 = bs1.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs1fnf1 = bs1.find_next(false, 1);
      ETL_CONSTEXPR14 size_t bs1fnt2 = bs1.find_next(true, 2);
      CHECK_EQUAL(0U, bs1fnf0);
      CHECK_EQUAL(1U, bs1fnf1);
      CHECK_EQUAL(etl::bitset<>::npos, bs1fnt2);

      ETL_CONSTEXPR14 etl::bitset<9> bs2(ull(0x1FF));
      ETL_CONSTEXPR14 size_t bs2fnt0 = bs2.find_next(true, 0);
      ETL_CONSTEXPR14 size_t bs2fnt1 = bs2.find_next(true, 1);
      ETL_CONSTEXPR14 size_t bs2fnf2 = bs2.find_next(false, 2);
      CHECK_EQUAL(0U, bs2fnt0);
      CHECK_EQUAL(1U, bs2fnt1);
      CHECK_EQUAL(etl::bitset<>::npos, bs2fnf2);

      ETL_CONSTEXPR14 etl::bitset<9> bs3(ull(0x000E));
      ETL_CONSTEXPR14 size_t bs3fnf0 = bs3.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs3fnt1 = bs3.find_next(true, 1);
      ETL_CONSTEXPR14 size_t bs3fnf2 = bs3.find_next(false, 2);
      CHECK_EQUAL(0U, bs3fnf0);
      CHECK_EQUAL(1U, bs3fnt1);
      CHECK_EQUAL(4U, bs3fnf2);

      ETL_CONSTEXPR14 etl::bitset<9> bs4(ull(0x0031));
      ETL_CONSTEXPR14 size_t bs4fnt0 = bs4.find_next(true, 0);
      ETL_CONSTEXPR14 size_t bs4fnf0 = bs4.find_next(false, 0);
      ETL_CONSTEXPR14 size_t bs4fnt1 = bs4.find_next(true, 1);
      CHECK_EQUAL(0U, bs4fnt0);
      CHECK_EQUAL(1U, bs4fnf0);
      CHECK_EQUAL(4U, bs4fnt1);
    }

    //*************************************************************************
    TEST(test_find_next_github_issue_617)
    {
      ETL_CONSTEXPR14 etl::bitset<64> bs1(ull(0xC000000000000031));
      ETL_CONSTEXPR14 size_t bs1fnt1 = bs1.find_next(true, 10);
      ETL_CONSTEXPR14 size_t bs1fnt2 = bs1.find_next(true, 59);
      CHECK_EQUAL(62U, bs1fnt1);
      CHECK_EQUAL(62U, bs1fnt2);

      ETL_CONSTEXPR14 etl::bitset<64> bs2(ull(0x3FFFFFFFFFFFFFCE));
      ETL_CONSTEXPR14 size_t bs2fnf1 = bs2.find_next(false, 10);
      ETL_CONSTEXPR14 size_t bs2fnf2 = bs2.find_next(false, 59);
      CHECK_EQUAL(62U, bs2fnf1);
      CHECK_EQUAL(62U, bs2fnf2);

      ETL_CONSTEXPR14 etl::bitset<64, uint32_t> bs32(ull(0xC000000000000031));
      ETL_CONSTEXPR14 size_t bs32fnt1 = bs32.find_next(true, 10);
      ETL_CONSTEXPR14 size_t bs32fnt2 = bs32.find_next(true, 59);
      CHECK_EQUAL(62U, bs32fnt1);
      CHECK_EQUAL(62U, bs32fnt2);
    }

    //*************************************************************************
    //*************************************************************************
    TEST(test_swap)
    {
      etl::bitset<9> compare1(0x2A);
      etl::bitset<9> compare2(0x15);
      etl::bitset<9> compare1a(0x2A);
      etl::bitset<9> compare2a(0x15);

      swap(compare1a, compare2a);

      CHECK(compare1 == compare2a);
      CHECK(compare2 == compare1a);
    }

    //*************************************************************************
    TEST(test_span)
    {
      using span_t = etl::bitset<32>::span_type;

      etl::bitset<32> b(0x12345678UL);

      span_t s = b.span();
      CHECK_EQUAL(0x78U, s[0]);
      CHECK_EQUAL(0x56U, s[1]);
      CHECK_EQUAL(0x34U, s[2]);
      CHECK_EQUAL(0x12U, s[3]);
    }

    //*************************************************************************
    TEST(test_const_span)
    {
      using span_t = etl::bitset<32>::const_span_type;

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
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      etl::string<32> text = b.to_string('.', '*');
      std::string stdtext  = b.to_string<std::string>('.', '*');

      CHECK_EQUAL("...*..*...**.*...*.*.**..****...", text.c_str());
      CHECK_EQUAL("...*..*...**.*...*.*.**..****...", stdtext.c_str());
    }

    //*************************************************************************
    TEST(test_to_wstring)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      etl::wstring<32> text = b.to_string<etl::wstring<32>>(L'.', L'*');
      std::wstring stdtext = b.to_string<std::wstring>(L'.', L'*');

      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(text.c_str()));
      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u16string)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      etl::u16string<32> text = b.to_string<etl::u16string<32>>(u'.', u'*');
      std::u16string stdtext = b.to_string<std::u16string>(u'.', u'*');

      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(text.c_str()));
      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u32string)
    {
      ETL_CONSTEXPR14 etl::bitset<32> b(0x12345678UL);

      etl::u32string<32> text = b.to_string<etl::u32string<32>>(U'.', U'*');
      std::u32string stdtext = b.to_string<std::u32string>(U'.', U'*');

      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(text.c_str()));
      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(stdtext.c_str()));
    }
  };
}
