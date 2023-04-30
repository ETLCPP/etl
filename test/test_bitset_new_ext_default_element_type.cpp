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

  SUITE(test_bitset_new_ext_default_element_type)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare;
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<14, int8_t>::buffer_type buffer14;
      etl::bitset_ext<30, int16_t>::buffer_type buffer30;
      etl::bitset_ext<62, int32_t>::buffer_type buffer62;

      etl::bitset_ext<14, int8_t>  data8(buffer14);
      etl::bitset_ext<30, int16_t> data16(buffer30);
      etl::bitset_ext<62, int32_t> data32(buffer62);

      CHECK_EQUAL(2U, data8.Number_Of_Elements);
      CHECK_EQUAL(2U, data16.Number_Of_Elements);
      CHECK_EQUAL(2U, data32.Number_Of_Elements);
    }

    //*************************************************************************
    TEST(test_default_constructor_default_element_type_bits)
    {
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<1>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<2>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<3>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<4>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<5>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<6>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<7>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<8>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<9>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<10>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<11>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<12>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<13>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<14>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<15>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<16>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<17>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<18>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<19>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<20>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<21>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<22>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<23>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<24>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<25>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<26>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<27>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<28>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<29>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<30>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<31>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<32>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<33>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<34>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<35>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<36>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<37>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<38>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<39>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<40>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<41>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<42>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<43>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<44>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<45>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<46>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<47>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<48>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<49>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<50>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<51>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<52>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<53>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<54>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<55>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<56>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<57>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<58>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<59>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<60>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<61>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<62>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<63>::element_type>::bits);
      CHECK_EQUAL(8U, etl::integral_limits<etl::bitset_ext<64>::element_type>::bits);
    }
     
    //*************************************************************************
    TEST(test_default_constructor_default_element_type_number_of_elements)
    {
      CHECK_EQUAL(1U, etl::bitset_ext<1>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset_ext<2>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset_ext<3>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset_ext<4>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset_ext<5>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset_ext<6>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset_ext<7>::Number_Of_Elements);
      CHECK_EQUAL(1U, etl::bitset_ext<8>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<9>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<10>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<11>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<12>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<13>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<14>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<15>::Number_Of_Elements);
      CHECK_EQUAL(2U, etl::bitset_ext<16>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<17>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<18>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<19>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<20>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<21>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<22>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<23>::Number_Of_Elements);
      CHECK_EQUAL(3U, etl::bitset_ext<24>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<25>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<26>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<27>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<28>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<29>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<30>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<31>::Number_Of_Elements);
      CHECK_EQUAL(4U, etl::bitset_ext<32>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<33>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<34>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<35>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<36>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<37>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<38>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<39>::Number_Of_Elements);
      CHECK_EQUAL(5U, etl::bitset_ext<40>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<41>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<42>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<43>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<44>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<45>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<46>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<47>::Number_Of_Elements);
      CHECK_EQUAL(6U, etl::bitset_ext<48>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<49>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<50>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<51>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<52>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<53>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<54>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<55>::Number_Of_Elements);
      CHECK_EQUAL(7U, etl::bitset_ext<56>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<57>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<58>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<59>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<60>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<61>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<62>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<63>::Number_Of_Elements);
      CHECK_EQUAL(8U, etl::bitset_ext<64>::Number_Of_Elements);
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int8_t_element_types_bits)
    {
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<1, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<2, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<3, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<4, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<5, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<6, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<7, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<8, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<9, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<10, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<11, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<12, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<13, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<14, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<15, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<16, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<17, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<18, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<19, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<20, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<21, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<22, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<23, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<24, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<25, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<26, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<27, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<28, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<29, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<30, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<31, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<32, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<33, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<34, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<35, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<36, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<37, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<38, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<39, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<40, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<41, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<42, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<43, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<44, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<45, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<46, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<47, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<48, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<49, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<50, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<51, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<52, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<53, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<54, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<55, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<56, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<57, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<58, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<59, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<60, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<61, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<62, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<63, int8_t>::element_type>::bits));
      CHECK_EQUAL(8U, (etl::integral_limits<etl::bitset_ext<64, int8_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int8_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset_ext<1, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<2, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<3, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<4, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<5, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<6, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<7, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<8, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<9, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<10, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<11, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<12, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<13, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<14, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<15, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<16, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<17, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<18, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<19, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<20, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<21, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<22, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<23, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<24, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<25, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<26, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<27, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<28, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<29, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<30, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<31, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<32, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<33, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<34, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<35, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<36, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<37, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<38, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<39, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(5U, (etl::bitset_ext<40, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<41, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<42, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<43, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<44, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<45, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<46, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<47, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(6U, (etl::bitset_ext<48, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<49, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<50, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<51, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<52, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<53, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<54, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<55, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(7U, (etl::bitset_ext<56, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<57, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<58, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<59, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<60, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<61, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<62, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<63, int8_t>::Number_Of_Elements));
      CHECK_EQUAL(8U, (etl::bitset_ext<64, int8_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int16_t_element_types_bits)
    {
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<1, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<2, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<3, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<4, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<5, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<6, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<7, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<8, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<9, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<10, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<11, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<12, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<13, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<14, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<15, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<16, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<17, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<18, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<19, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<20, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<21, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<22, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<23, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<24, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<25, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<26, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<27, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<28, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<29, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<30, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<31, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<32, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<33, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<34, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<35, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<36, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<37, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<38, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<39, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<40, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<41, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<42, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<43, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<44, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<45, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<46, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<47, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<48, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<49, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<50, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<51, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<52, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<53, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<54, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<55, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<56, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<57, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<58, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<59, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<60, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<61, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<62, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<63, int16_t>::element_type>::bits));
      CHECK_EQUAL(16U, (etl::integral_limits<etl::bitset_ext<64, int16_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int16_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset_ext<1, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<2, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<3, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<4, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<5, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<6, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<7, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<8, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<9, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<10, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<11, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<12, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<13, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<14, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<15, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<16, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<17, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<18, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<19, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<20, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<21, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<22, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<23, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<24, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<25, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<26, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<27, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<28, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<29, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<30, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<31, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<32, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<33, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<34, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<35, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<36, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<37, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<38, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<39, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<40, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<41, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<42, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<43, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<44, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<45, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<46, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<47, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(3U, (etl::bitset_ext<48, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<49, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<50, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<51, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<52, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<53, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<54, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<55, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<56, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<57, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<58, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<59, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<60, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<61, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<62, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<63, int16_t>::Number_Of_Elements));
      CHECK_EQUAL(4U, (etl::bitset_ext<64, int16_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int32_t_element_types_bits)
    {
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<1, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<2, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<3, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<4, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<5, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<6, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<7, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<8, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<9, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<10, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<11, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<12, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<13, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<14, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<15, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<16, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<17, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<18, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<19, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<20, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<21, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<22, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<23, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<24, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<25, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<26, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<27, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<28, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<29, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<30, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<31, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<32, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<33, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<34, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<35, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<36, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<37, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<38, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<39, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<40, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<41, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<42, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<43, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<44, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<45, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<46, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<47, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<48, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<49, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<50, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<51, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<52, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<53, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<54, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<55, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<56, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<57, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<58, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<59, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<60, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<61, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<62, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<63, int32_t>::element_type>::bits));
      CHECK_EQUAL(32U, (etl::integral_limits<etl::bitset_ext<64, int32_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int32_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset_ext<1, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<2, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<3, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<4, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<5, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<6, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<7, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<8, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<9, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<10, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<11, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<12, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<13, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<14, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<15, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<16, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<17, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<18, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<19, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<20, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<21, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<22, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<23, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<24, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<25, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<26, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<27, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<28, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<29, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<30, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<31, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<32, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<33, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<34, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<35, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<36, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<37, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<38, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<39, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<40, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<41, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<42, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<43, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<44, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<45, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<46, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<47, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<48, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<49, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<50, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<51, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<52, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<53, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<54, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<55, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<56, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<57, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<58, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<59, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<60, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<61, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<62, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<63, int32_t>::Number_Of_Elements));
      CHECK_EQUAL(2U, (etl::bitset_ext<64, int32_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int64_t_element_types_bits)
    {
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<1, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<2, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<3, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<4, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<5, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<6, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<7, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<8, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<9, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<10, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<11, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<12, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<13, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<14, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<15, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<16, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<17, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<18, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<19, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<20, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<21, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<22, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<23, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<24, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<25, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<26, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<27, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<28, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<29, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<30, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<31, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<32, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<33, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<34, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<35, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<36, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<37, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<38, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<39, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<40, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<41, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<42, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<43, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<44, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<45, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<46, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<47, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<48, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<49, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<50, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<51, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<52, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<53, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<54, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<55, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<56, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<57, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<58, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<59, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<60, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<61, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<62, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<63, int64_t>::element_type>::bits));
      CHECK_EQUAL(64U, (etl::integral_limits<etl::bitset_ext<64, int64_t>::element_type>::bits));
    }

    //*************************************************************************
    TEST(test_default_constructor_explicit_int64_t_element_types_number_of_elements)
    {
      CHECK_EQUAL(1U, (etl::bitset_ext<1, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<2, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<3, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<4, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<5, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<6, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<7, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<8, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<9, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<10, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<11, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<12, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<13, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<14, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<15, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<16, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<17, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<18, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<19, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<20, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<21, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<22, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<23, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<24, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<25, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<26, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<27, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<28, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<29, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<30, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<31, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<32, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<33, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<34, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<35, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<36, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<37, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<38, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<39, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<40, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<41, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<42, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<43, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<44, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<45, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<46, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<47, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<48, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<49, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<50, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<51, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<52, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<53, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<54, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<55, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<56, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<57, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<58, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<59, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<60, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<61, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<62, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<63, int64_t>::Number_Of_Elements));
      CHECK_EQUAL(1U, (etl::bitset_ext<64, int64_t>::Number_Of_Elements));
    }

    //*************************************************************************
    TEST(test_construct_from_value)
    {
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(0x123456731234567ULL);
      etl::bitset_ext<60> data(0x123456731234567ULL, buffer);

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
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;

      std::bitset<60> compare(0x123456731234567ULL);
      etl::bitset_ext<60> data(0x123456731234567ULL, buffer1);
      etl::bitset_ext<60> data_copy(data, buffer2);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(0x8765432187654321ULL);
      etl::bitset_ext<60> data(0x8765432187654321ULL, buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data("110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(L"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(u"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(U"110001001000110100010101100111001100010010001101000101011001", buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001111100001");
      etl::bitset_ext<60> data("110001001000110100010101100111001100010010001101000101011001111100001", buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare;
      etl::bitset_ext<60> data(buffer);

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
    TEST(test_char_string_set)
    {
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(L"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(u"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(U"110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare("110001001000110100010101100111001100010010001101000101011001");
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<16>::buffer_type buffer;

      etl::bitset_ext<16> data((unsigned long long)etl::integral_limits<uint16_t>::min, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u16_max)
    {
      etl::bitset_ext<16>::buffer_type buffer;

      etl::bitset_ext<16> data((unsigned long long)etl::integral_limits<uint16_t>::max, buffer);
      uint16_t value = data.value<uint16_t>();

      CHECK_EQUAL(std::numeric_limits<uint16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_min)
    {
      etl::bitset_ext<16>::buffer_type buffer;

      etl::bitset_ext<16> data((unsigned long long)etl::integral_limits<int16_t>::min, buffer);
      int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s16_max)
    {
      etl::bitset_ext<16>::buffer_type buffer;

      etl::bitset_ext<16> data((unsigned long long)etl::integral_limits<int16_t>::max, buffer);
      int16_t value = data.value<int16_t>();

      CHECK_EQUAL(std::numeric_limits<int16_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_min)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> data((unsigned long long)etl::integral_limits<uint32_t>::min, buffer);
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u32_max)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> data((unsigned long long)etl::integral_limits<uint32_t>::max, buffer);
      uint32_t value = data.value<uint32_t>();

      CHECK_EQUAL(std::numeric_limits<uint32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_min)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> data((unsigned long long)etl::integral_limits<int32_t>::min, buffer);
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s32_max)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> data((unsigned long long)etl::integral_limits<int32_t>::max, buffer);
      int32_t value = data.value<int32_t>();

      CHECK_EQUAL(std::numeric_limits<int32_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_min)
    {
      etl::bitset_ext<64>::element_type buffer[etl::bitset_ext<64>::Number_Of_Elements];

      etl::bitset_ext<64> data((unsigned long long)etl::integral_limits<uint64_t>::min, buffer);
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_u64_max)
    {
      etl::bitset_ext<64>::element_type buffer[etl::bitset_ext<64>::Number_Of_Elements];

      etl::bitset_ext<64> data((unsigned long long)etl::integral_limits<uint64_t>::max, buffer);
      uint64_t value = data.value<uint64_t>();

      CHECK_EQUAL(std::numeric_limits<uint64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_min)
    {
      etl::bitset_ext<64>::element_type buffer[etl::bitset_ext<64>::Number_Of_Elements];

      etl::bitset_ext<64> data((unsigned long long)etl::integral_limits<int64_t>::min, buffer);
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::min(), value);
    }

    //*************************************************************************
    TEST(test_value_s64_max)
    {
      etl::bitset_ext<64>::element_type buffer[etl::bitset_ext<64>::Number_Of_Elements];

      etl::bitset_ext<64> data((unsigned long long)etl::integral_limits<int64_t>::max, buffer);
      int64_t value = data.value<int64_t>();

      CHECK_EQUAL(std::numeric_limits<int64_t>::max(), value);
    }

    //*************************************************************************
    TEST(test_position_set)
    {
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare;
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<60> data(0xFFFFFFFFFFFFFFFULL, buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<60> data(0xFFFFFFFFFFFFFFFULL, buffer);

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
    TEST(test_index_operator_read)
    {
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare(0x3123456731234567ULL);
      etl::bitset_ext<60> data(0x3123456731234567ULL, buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;
      etl::bitset_ext<60>::buffer_type buffer4;

      etl::bitset_ext<60> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60> data3(ull(0x0000010001000100), buffer3);
      etl::bitset_ext<60> data4(ull(0x0FFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_none)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;
      etl::bitset_ext<60>::buffer_type buffer4;

      etl::bitset_ext<60> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60> data3(ull(0x0000010001000100), buffer3);
      etl::bitset_ext<60> data4(ull(0x0FFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_all)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;
      etl::bitset_ext<60>::buffer_type buffer4;

      etl::bitset_ext<60> data1(ull(0x0000000000000000), buffer1);
      etl::bitset_ext<60> data2(ull(0x0000010000000000), buffer2);
      etl::bitset_ext<60> data3(ull(0x0000010001000100), buffer3);
      etl::bitset_ext<60> data4(ull(0x0FFFFFFFFFFFFFFF), buffer4);

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
    TEST(test_flip)
    {
      etl::bitset_ext<60>::buffer_type buffer;
      std::bitset<60> compare;
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;
      std::bitset<60> compare;
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;
      std::bitset<60> compare;
      etl::bitset_ext<60> data(buffer);

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
      etl::bitset_ext<60>::buffer_type buffer;

      std::bitset<60> compare;
      etl::bitset_ext<60> data(buffer);

      bool bc = ~compare[3];
      bool bd = ~data[3];
      CHECK_EQUAL(bc, bd);
    }

    //*************************************************************************
    void test_assignment_operator_helper(etl::bitset_ext<60>& from, etl::bitset_ext<60>& to)
    {
      to = from;
    }

    TEST(test_assignment_operator)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;

      std::bitset<60> compare(0xFFFFFFFFFFFFFFFULL);
      etl::bitset_ext<60> data1(0xFFFFFFFFFFFFFFFULL, buffer1);
      etl::bitset_ext<60> data2(buffer2);

      test_assignment_operator_helper(data1, data2);

      for (size_t i = 0UL; i < data2.size(); ++i)
      {
        CHECK_EQUAL(compare.test(i), data2.test(i));
      }
    }

    //*************************************************************************
    TEST(test_equality_operator)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;

      etl::bitset_ext<60> data1(0x123456781234567ULL, buffer1);
      etl::bitset_ext<60> data2(0x123456781234567ULL, buffer2);
      etl::bitset_ext<60> data3(buffer3);

      bool equal     =  (data1 == data2);
      bool not_equal = !(data1 == data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_inequality_operator)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;

      etl::bitset_ext<60> data1(0x123456781234567ULL, buffer1);
      etl::bitset_ext<60> data2(0x123456781234567ULL, buffer2);
      etl::bitset_ext<60> data3(buffer3);

      bool equal = !(data1 != data2);
      bool not_equal = (data1 != data3);

      CHECK(equal);
      CHECK(not_equal);
    }

    //*************************************************************************
    TEST(test_shift_left_operator_all_shifts_full_size)
    {
      int64_t value = 0x0123456789ABCDEFULL;
      uint64_t mask = 0xFFFFFFFFFFFFFFFFULL;

      etl::bitset_ext<64>::buffer_type buffer1;
      etl::bitset_ext<64>::buffer_type buffer2;

      etl::bitset_ext<64> original(0x0123456789ABCDEFULL, buffer1);

      for (ull shift = 0U; shift < 64U; ++shift)
      {
        etl::bitset_ext<64> data(original, buffer2);
        CHECK_EQUAL_HEX(((value & mask) << shift), (data <<= shift).value<int64_t>());
        mask >>= 1;
      }

      etl::bitset_ext<64> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data <<= 64U).value<int64_t>());
    }

    //*************************************************************************
    TEST(test_shift_left_operator_all_shifts_partial_size)
    {
      int64_t value = 0x0123456789ABCDEFULL;
      uint64_t mask = 0x0FFFFFFFFFFFFFFFULL;

      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;

      etl::bitset_ext<60> original(0x0123456789ABCDEFULL, buffer1);

      for (ull shift = 0U; shift < 64U; ++shift)
      {        
        etl::bitset_ext<60> data(original, buffer2);
        CHECK_EQUAL_HEX(((value & mask) << shift), (data <<= shift).value<int64_t>());
        mask >>= 1;
      }

      etl::bitset_ext<60> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data <<= 64U).value<int64_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_full_size)
    {
      int64_t value = 0x0123456789ABCDEFULL;

      etl::bitset_ext<64>::buffer_type buffer1;
      etl::bitset_ext<64>::buffer_type buffer2;

      etl::bitset_ext<64> original(0x0123456789ABCDEFULL, buffer1);

      for (ull shift = 0U; shift < 64U; ++shift)
      {
        etl::bitset_ext<64> data(original, buffer2);
        CHECK_EQUAL_HEX((value >> shift), (data >>= shift).value<int64_t>());
      }

      etl::bitset_ext<64> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data >>= 64U).value<int64_t>());
    }

    //*************************************************************************
    TEST(test_shift_right_operator_all_shifts_partial_size)
    {
      int64_t value = 0x0123456789ABCDEFULL;

      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;

      etl::bitset_ext<60> original(0x0123456789ABCDEFULL, buffer1);

      for (ull shift = 0U; shift < 64U; ++shift)
      {
        etl::bitset_ext<60> data(original, buffer2);
        CHECK_EQUAL_HEX((value >> shift), (data >>= shift).value<int64_t>());
      }

      etl::bitset_ext<60> data(original, buffer2);
      CHECK_EQUAL_HEX(0ULL, (data >>= 64U).value<int64_t>());
    }

    //*************************************************************************
    TEST(test_and_equals_operator)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;
      etl::bitset_ext<60>::buffer_type buffer4;

      etl::bitset_ext<60> data1(0x12345678UL, buffer1);
      etl::bitset_ext<60> data2(0x23456789UL, buffer2);
      etl::bitset_ext<60> data3(0x12345678UL & 0x23456789UL, buffer3);
      
      etl::bitset_ext<60> data4(data1, buffer4);
      data4 &= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_or_equals_operator)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;
      etl::bitset_ext<60>::buffer_type buffer4;

      etl::bitset_ext<60> data1(0x12345678UL, buffer1);
      etl::bitset_ext<60> data2(0x23456789UL, buffer2);
      etl::bitset_ext<60> data3(0x12345678UL | 0x23456789UL, buffer3);

      etl::bitset_ext<60> data4(data1, buffer4);
      data4 |= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_xor_equals_operator)
    {
      etl::bitset_ext<60>::buffer_type buffer1;
      etl::bitset_ext<60>::buffer_type buffer2;
      etl::bitset_ext<60>::buffer_type buffer3;
      etl::bitset_ext<60>::buffer_type buffer4;

      etl::bitset_ext<60> data1(0x12345678UL, buffer1);
      etl::bitset_ext<60> data2(0x23456789UL, buffer2);
      etl::bitset_ext<60> data3(0x12345678UL ^ 0x23456789UL, buffer3);

      etl::bitset_ext<60> data4(data1, buffer4);
      data4 ^= data2;
      CHECK(data3 == data4);
    }

    //*************************************************************************
    TEST(test_find_first)
    {
      etl::bitset_ext<9>::element_type buffer1[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer2[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer3[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer4[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer5[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer6[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer7[etl::bitset_ext<9>::Number_Of_Elements];

      etl::bitset_ext<9> bs1(ull(0x0000), buffer1);
      size_t bs1fff = bs1.find_first(false);
      size_t bs1fft = bs1.find_first(true);
      CHECK_EQUAL(0U, bs1fff);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs1fft);

      etl::bitset_ext<9> bs2(ull(0x01FF), buffer2);
      size_t bs2fff = bs2.find_first(false);
      size_t bs2fft = bs2.find_first(true);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs2fff);
      CHECK_EQUAL(0U, bs2fft);

      etl::bitset_ext<9> bs3(ull(0x0001), buffer3);
      size_t bs3fff = bs3.find_first(false);
      size_t bs3fft = bs3.find_first(true);
      CHECK_EQUAL(1U, bs3fff);
      CHECK_EQUAL(0U, bs3fft);

      etl::bitset_ext<9> bs4(ull(0x0020), buffer4);
      size_t bs4fff = bs4.find_first(false);
      size_t bs4fft = bs4.find_first(true);
      CHECK_EQUAL(0U, bs4fff);
      CHECK_EQUAL(5U, bs4fft);

      etl::bitset_ext<9> bs5(ull(0x0021), buffer5);
      size_t bs5fff = bs5.find_first(false);
      size_t bs5fft = bs5.find_first(true);
      CHECK_EQUAL(1U, bs5fff);
      CHECK_EQUAL(0U, bs5fft);

      etl::bitset_ext<9> bs6(ull(0x000E), buffer6);
      size_t bs6fff = bs6.find_first(false);
      size_t bs6fft = bs6.find_first(true);
      CHECK_EQUAL(0U, bs6fff);
      CHECK_EQUAL(1U, bs6fft);

      etl::bitset_ext<9> bs7(ull(0x31), buffer7);
      size_t bs7fff = bs7.find_first(false);
      size_t bs7fft = bs7.find_first(true);
      CHECK_EQUAL(1U, bs7fff);
      CHECK_EQUAL(0U, bs7fft);
    }
     
    //*************************************************************************
    TEST(test_find_next)
    {
      etl::bitset_ext<9>::element_type buffer1[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer2[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer3[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer4[etl::bitset_ext<9>::Number_Of_Elements];

      etl::bitset_ext<9> bs1(ull(0x0000), buffer1);
      size_t bs1fnf0 = bs1.find_next(false, 0);
      size_t bs1fnf1 = bs1.find_next(false, 1);
      size_t bs1fnt2 = bs1.find_next(true, 2);
      CHECK_EQUAL(0U, bs1fnf0);
      CHECK_EQUAL(1U, bs1fnf1);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs1fnt2);

      etl::bitset_ext<9> bs2(ull(0x1FF), buffer2);
      size_t bs2fnt0 = bs2.find_next(true, 0);
      size_t bs2fnt1 = bs2.find_next(true, 1);
      size_t bs2fnf2 = bs2.find_next(false, 2);
      CHECK_EQUAL(0U, bs2fnt0);
      CHECK_EQUAL(1U, bs2fnt1);
      CHECK_EQUAL(etl::bitset_ext<>::npos, bs2fnf2);

      etl::bitset_ext<9> bs3(ull(0x000E), buffer3);
      size_t bs3fnf0 = bs3.find_next(false, 0);
      size_t bs3fnt1 = bs3.find_next(true, 1);
      size_t bs3fnf2 = bs3.find_next(false, 2);
      CHECK_EQUAL(0U, bs3fnf0);
      CHECK_EQUAL(1U, bs3fnt1);
      CHECK_EQUAL(4U, bs3fnf2);

      etl::bitset_ext<9> bs4(ull(0x0031), buffer4);
      size_t bs4fnt0 = bs4.find_next(true, 0);
      size_t bs4fnf0 = bs4.find_next(false, 0);
      size_t bs4fnt1 = bs4.find_next(true, 1);
      CHECK_EQUAL(0U, bs4fnt0);
      CHECK_EQUAL(1U, bs4fnf0);
      CHECK_EQUAL(4U, bs4fnt1);
    }

    //*************************************************************************
    TEST(test_find_next_github_issue_617)
    {
      etl::bitset_ext<64>::element_type buffer1[etl::bitset_ext<64>::Number_Of_Elements];
      etl::bitset_ext<64>::element_type buffer2[etl::bitset_ext<64>::Number_Of_Elements];

      etl::bitset_ext<64> bs1(ull(0xC000000000000031), buffer1);
      size_t bs1fnt1 = bs1.find_next(true, 10);
      size_t bs1fnt2 = bs1.find_next(true, 59);
      CHECK_EQUAL(62U, bs1fnt1);
      CHECK_EQUAL(62U, bs1fnt2);

      etl::bitset_ext<64> bs2(ull(0x3FFFFFFFFFFFFFCE), buffer2);
      size_t bs2fnf1 = bs2.find_next(false, 10);
      size_t bs2fnf2 = bs2.find_next(false, 59);
      CHECK_EQUAL(62U, bs2fnf1);
      CHECK_EQUAL(62U, bs2fnf2);
    }

    //*************************************************************************
    TEST(test_swap)
    {
      etl::bitset_ext<9>::element_type buffer1[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer2[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer3[etl::bitset_ext<9>::Number_Of_Elements];
      etl::bitset_ext<9>::element_type buffer4[etl::bitset_ext<9>::Number_Of_Elements];

      etl::bitset_ext<9> compare1(0x2A, buffer1);
      etl::bitset_ext<9> compare2(0x15, buffer2);
      etl::bitset_ext<9> compare1a(0x2A, buffer3);
      etl::bitset_ext<9> compare2a(0x15, buffer4);

      swap(compare1a, compare2a);

      CHECK(compare1 == compare2a);
      CHECK(compare2 == compare1a);
    }

    //*************************************************************************
    TEST(test_span)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      using span_t = etl::bitset_ext<32>::span_type;

      etl::bitset_ext<32> data(0x12345678UL, buffer);

      span_t s = data.span();
      CHECK_EQUAL(0x78U, s[0]);
      CHECK_EQUAL(0x56U, s[1]);
      CHECK_EQUAL(0x34U, s[2]);
      CHECK_EQUAL(0x12U, s[3]);
    }

    //*************************************************************************
    TEST(test_const_span)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      using span_t = etl::bitset_ext<32>::const_span_type;

      const etl::bitset_ext<32> b(0x12345678UL, buffer);

      span_t s = b.span();
      CHECK_EQUAL(0x78U, s[0]);
      CHECK_EQUAL(0x56U, s[1]);
      CHECK_EQUAL(0x34U, s[2]);
      CHECK_EQUAL(0x12U, s[3]);
    }

    //*************************************************************************
    TEST(test_to_string)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> b(0x12345678UL, buffer);

      etl::string<32> text = b.to_string('.', '*');
      std::string stdtext  = b.to_string<std::string>('.', '*');

      CHECK_EQUAL("...*..*...**.*...*.*.**..****...", text.c_str());
      CHECK_EQUAL("...*..*...**.*...*.*.**..****...", stdtext.c_str());
    }

    //*************************************************************************
    TEST(test_to_wstring)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> b(0x12345678UL, buffer);

      etl::wstring<32> text = b.to_string<etl::wstring<32>>(L'.', L'*');
      std::wstring stdtext = b.to_string<std::wstring>(L'.', L'*');

      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(text.c_str()));
      CHECK(std::wstring(L"...*..*...**.*...*.*.**..****...") == std::wstring(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u16string)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> b(0x12345678UL, buffer);

      etl::u16string<32> text = b.to_string<etl::u16string<32>>(u'.', u'*');
      std::u16string stdtext = b.to_string<std::u16string>(u'.', u'*');

      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(text.c_str()));
      CHECK(std::u16string(u"...*..*...**.*...*.*.**..****...") == std::u16string(stdtext.c_str()));
    }

    //*************************************************************************
    TEST(test_to_u32string)
    {
      etl::bitset_ext<32>::element_type buffer[etl::bitset_ext<32>::Number_Of_Elements];

      etl::bitset_ext<32> b(0x12345678UL, buffer);

      etl::u32string<32> text = b.to_string<etl::u32string<32>>(U'.', U'*');
      std::u32string stdtext = b.to_string<std::u32string>(U'.', U'*');

      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(text.c_str()));
      CHECK(std::u32string(U"...*..*...**.*...*.*.**..****...") == std::u32string(stdtext.c_str()));
    }
  };
}
