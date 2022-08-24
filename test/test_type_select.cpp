/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 John Wellbelove

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

#include "etl/type_select.h"
#include "etl/null_type.h"

#include <type_traits>

namespace
{
  typedef etl::type_select<etl::null_type<0>> Types1;

  typedef etl::type_select<etl::null_type<0>, etl::null_type<1>, etl::null_type<2>, etl::null_type<3>,
                           etl::null_type<4>, etl::null_type<5>, etl::null_type<6>, etl::null_type<7>> Types8;

  typedef etl::type_select<etl::null_type<0>,  etl::null_type<1>,  etl::null_type<2>,  etl::null_type<3>,
                           etl::null_type<4>,  etl::null_type<5>,  etl::null_type<6>,  etl::null_type<7>,
                           etl::null_type<8>,  etl::null_type<9>,  etl::null_type<10>, etl::null_type<11>,
                           etl::null_type<12>, etl::null_type<13>, etl::null_type<14>, etl::null_type<15>> Types16;

  SUITE(test_type_lookup)
  {
    //*************************************************************************
    TEST(test_type_select_1)
    {
      CHECK((etl::is_same<etl::null_type<0>, typename Types1::select<0>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types1::select<0>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_select_t_1)
    {
      CHECK((etl::is_same<etl::null_type<0>, typename Types1::select_t<0>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types1::select_t<0>>::value));
    }
#endif

    //*************************************************************************
    TEST(test_type_select_8)
    {
      CHECK((etl::is_same<etl::null_type<0>,   typename Types8::select<0>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<0>::type>::value));
      CHECK((etl::is_same<etl::null_type<1>,   typename Types8::select<1>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<1>::type>::value));
      CHECK((etl::is_same<etl::null_type<2>,   typename Types8::select<2>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<2>::type>::value));
      CHECK((etl::is_same<etl::null_type<3>,   typename Types8::select<3>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<3>::type>::value));
      CHECK((etl::is_same<etl::null_type<4>,   typename Types8::select<4>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<4>::type>::value));
      CHECK((etl::is_same<etl::null_type<5>,   typename Types8::select<5>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<5>::type>::value));
      CHECK((etl::is_same<etl::null_type<6>,   typename Types8::select<6>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<6>::type>::value));
      CHECK((etl::is_same<etl::null_type<7>,   typename Types8::select<7>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select<7>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_select_t_8)
    {
      CHECK((etl::is_same<etl::null_type<0>, typename Types8::select_t<0>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<0>>::value));
      CHECK((etl::is_same<etl::null_type<1>, typename Types8::select_t<1>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<1>>::value));
      CHECK((etl::is_same<etl::null_type<2>, typename Types8::select_t<2>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<2>>::value));
      CHECK((etl::is_same<etl::null_type<3>, typename Types8::select_t<3>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<3>>::value));
      CHECK((etl::is_same<etl::null_type<4>, typename Types8::select_t<4>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<4>>::value));
      CHECK((etl::is_same<etl::null_type<5>, typename Types8::select_t<5>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<5>>::value));
      CHECK((etl::is_same<etl::null_type<6>, typename Types8::select_t<6>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<6>>::value));
      CHECK((etl::is_same<etl::null_type<7>, typename Types8::select_t<7>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types8::select_t<7>>::value));
    }
#endif

    //*************************************************************************
    TEST(test_type_select_16)
    {
      CHECK((etl::is_same<etl::null_type<0>,   typename Types16::select<0>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<0>::type>::value));
      CHECK((etl::is_same<etl::null_type<1>,   typename Types16::select<1>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<1>::type>::value));
      CHECK((etl::is_same<etl::null_type<2>,   typename Types16::select<2>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<2>::type>::value));
      CHECK((etl::is_same<etl::null_type<3>,   typename Types16::select<3>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<3>::type>::value));
      CHECK((etl::is_same<etl::null_type<4>,   typename Types16::select<4>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<4>::type>::value));
      CHECK((etl::is_same<etl::null_type<5>,   typename Types16::select<5>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<5>::type>::value));
      CHECK((etl::is_same<etl::null_type<6>,   typename Types16::select<6>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<6>::type>::value));
      CHECK((etl::is_same<etl::null_type<7>,   typename Types16::select<7>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<7>::type>::value));
      CHECK((etl::is_same<etl::null_type<8>,   typename Types16::select<8>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<8>::type>::value));
      CHECK((etl::is_same<etl::null_type<9>,   typename Types16::select<9>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<9>::type>::value));
      CHECK((etl::is_same<etl::null_type<10>,  typename Types16::select<10>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<10>::type>::value));
      CHECK((etl::is_same<etl::null_type<11>,  typename Types16::select<11>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<11>::type>::value));
      CHECK((etl::is_same<etl::null_type<12>,  typename Types16::select<12>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<12>::type>::value));
      CHECK((etl::is_same<etl::null_type<13>,  typename Types16::select<13>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<13>::type>::value));
      CHECK((etl::is_same<etl::null_type<14>,  typename Types16::select<14>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<14>::type>::value));
      CHECK((etl::is_same<etl::null_type<15>,  typename Types16::select<15>::type>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select<15>::type>::value));
    }

#if !defined(ETL_TYPE_SELECT_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_type_select_t_16)
    {
      CHECK((etl::is_same<etl::null_type<0>, typename Types16::select_t<0>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<0>>::value));
      CHECK((etl::is_same<etl::null_type<1>, typename Types16::select_t<1>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<1>>::value));
      CHECK((etl::is_same<etl::null_type<2>, typename Types16::select_t<2>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<2>>::value));
      CHECK((etl::is_same<etl::null_type<3>, typename Types16::select_t<3>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<3>>::value));
      CHECK((etl::is_same<etl::null_type<4>, typename Types16::select_t<4>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<4>>::value));
      CHECK((etl::is_same<etl::null_type<5>, typename Types16::select_t<5>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<5>>::value));
      CHECK((etl::is_same<etl::null_type<6>, typename Types16::select_t<6>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<6>>::value));
      CHECK((etl::is_same<etl::null_type<7>, typename Types16::select_t<7>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<7>>::value));
      CHECK((etl::is_same<etl::null_type<8>, typename Types16::select_t<8>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<8>>::value));
      CHECK((etl::is_same<etl::null_type<9>, typename Types16::select_t<9>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<9>>::value));
      CHECK((etl::is_same<etl::null_type<10>, typename Types16::select_t<10>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<10>>::value));
      CHECK((etl::is_same<etl::null_type<11>, typename Types16::select_t<11>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<11>>::value));
      CHECK((etl::is_same<etl::null_type<12>, typename Types16::select_t<12>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<12>>::value));
      CHECK((etl::is_same<etl::null_type<13>, typename Types16::select_t<13>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<13>>::value));
      CHECK((etl::is_same<etl::null_type<14>, typename Types16::select_t<14>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<14>>::value));
      CHECK((etl::is_same<etl::null_type<15>, typename Types16::select_t<15>>::value));
      CHECK(!(etl::is_same<etl::null_type<99>, typename Types16::select_t<15>>::value));
    }
#endif
  };
}
