/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#include <stdint.h>

#include "etl/type_def.h"

#include "etl/type_traits.h"

namespace
{
  SUITE(test_type_def)
  {
    //*************************************************************************
    TEST(test_macro)
    {
      ETL_TYPEDEF(uint32_t, type1_t);
      ETL_TYPEDEF(uint32_t, type2_t);

      type1_t t1 = type1_t(1);
      type2_t t2 = type2_t(1);

      uint32_t i1 = t1.get();
      uint32_t i2 = t2.get();

      CHECK_EQUAL(i1, i2);
      CHECK_TRUE(std::is_trivially_copyable<type1_t>::value);
    }

    //*************************************************************************
    TEST(test_macro_constexpr)
    {
      ETL_TYPEDEF(uint32_t, type1_t);
      ETL_TYPEDEF(uint32_t, type2_t);

      constexpr type1_t t1 = type1_t(1);
      constexpr type2_t t2 = type2_t(1);

      uint32_t i1 = t1.get();
      uint32_t i2 = t2.get();

      CHECK_EQUAL(i1, i2);
    }

    //*************************************************************************
    TEST(test_implicit)
    {
      class type1_t_tag;
      typedef etl::type_def<type1_t_tag, uint32_t> type1_t;

      class type2_t_tag;
      typedef etl::type_def<type2_t_tag, uint32_t> type2_t;

      type1_t t1 = type1_t(1);
      type2_t t2 = type2_t(1);

      uint32_t i1 = t1.get();
      uint32_t i2 = t2.get();

      CHECK_EQUAL(i1, i2);
    }

    //*************************************************************************
    TEST(test_implicit_constexpr)
    {
      class type1_t_tag;
      typedef etl::type_def<type1_t_tag, uint32_t> type1_t;

      class type2_t_tag;
      typedef etl::type_def<type2_t_tag, uint32_t> type2_t;

      constexpr type1_t t1 = type1_t(1);
      constexpr type2_t t2 = type2_t(1);

      uint32_t i1 = t1.get();
      uint32_t i2 = t2.get();

      CHECK_EQUAL(i1, i2);
    }

    //*************************************************************************
    TEST(test_get)
    {
      class type1_t_tag;
      typedef etl::type_def<type1_t_tag, uint32_t> type1_t;

      class type2_t_tag;
      typedef etl::type_def<type2_t_tag, uint32_t> type2_t;

      type1_t t1(1);
      type2_t t2(1);

      CHECK_EQUAL(t1.get(), t2.get());
    }

    //*************************************************************************
    TEST(test_get_constexpr)
    {
      class type1_t_tag;
      typedef etl::type_def<type1_t_tag, uint32_t> type1_t;

      class type2_t_tag;
      typedef etl::type_def<type2_t_tag, uint32_t> type2_t;

      constexpr type1_t t1(1);
      constexpr type2_t t2(1);

      CHECK_EQUAL(t1.get(), t2.get());
    }

    //*************************************************************************
    TEST(test_operators)
    {
      class __type_t__;
      typedef etl::type_def<__type_t__, uint32_t> type_t;

      uint32_t i = 0x5A3DUL;
      type_t t(0x5A3DUL);

      CHECK_EQUAL(++i, uint32_t(++t));
      CHECK_EQUAL(i++, uint32_t(t++));
      CHECK_EQUAL(--i, uint32_t(--t));
      CHECK_EQUAL(i--, uint32_t(t--));
      CHECK_EQUAL(i += 2, uint32_t(t += 2));
      CHECK_EQUAL(i += 2, uint32_t(t += type_t(2)));
      CHECK_EQUAL(i -= 2, uint32_t(t -= 2));
      CHECK_EQUAL(i -= 2, uint32_t(t -= type_t(2)));
      CHECK_EQUAL(i *= 2, uint32_t(t *= 2));
      CHECK_EQUAL(i *= 2, uint32_t(t *= type_t(2)));
      CHECK_EQUAL(i /= 2, uint32_t(t /= 2));
      CHECK_EQUAL(i /= 2, uint32_t(t /= type_t(2)));
      CHECK_EQUAL(i &= 0xFF00U, uint32_t(t &= 0xFF00U));
      CHECK_EQUAL(i &= 0xFF00U, uint32_t(t &= type_t(0xFF00U)));
      CHECK_EQUAL(i |= 0x003DU, uint32_t(t |= 0x003DU));
      CHECK_EQUAL(i |= 0x003DU, uint32_t(t |= type_t(0x003DU)));
      CHECK_EQUAL(i ^= 0xAA55U, uint32_t(t ^= 0xAA55U));
      CHECK_EQUAL(i ^= 0xAA55U, uint32_t(t ^= type_t(0xAA55U)));
      CHECK_EQUAL(i <<= 2U, uint32_t(t <<= 2U));
      CHECK_EQUAL(i >>= 2U, uint32_t(t >>= 2U));
      CHECK_EQUAL(i %= 23, uint32_t(t %= 23));

      t = type_t(0x1234U);
      CHECK_EQUAL(0x1234U, uint32_t(t));
    }

    //*************************************************************************
    TEST(test_comparisons)
    {
      class __type_t__;
      typedef etl::type_def<__type_t__, uint32_t> type_t;

      type_t t1(1);
      type_t t2(2);
      type_t t3(t1);
      type_t t4(t2);

      CHECK(t1 <  t2);
      CHECK(!(t2 <  t1));
      CHECK(t1 <= t2);
      CHECK(!(t2 <= t1));
      CHECK(t1 <= t3);
      CHECK(t2 >  t1);
      CHECK(!(t1 >  t2));
      CHECK(t2 >= t1);
      CHECK(!(t1 >= t2));
      CHECK(t2 >= t4);
    }

    //*************************************************************************
    TEST(test_comparisons_constexpr)
    {
      class __type_t__;
      typedef etl::type_def<__type_t__, uint32_t> type_t;

      constexpr type_t t1(1);
      constexpr type_t t2(2);

      constexpr bool eq   = t1 == t1;
      constexpr bool neq  = t1 == t2;
      constexpr bool lt   = t1  < t2;
      constexpr bool nlt  = t2  < t1;
      constexpr bool lte  = t1 <= t2;
      constexpr bool nlte = t2 <= t1;
      constexpr bool gt   = t1  > t2;
      constexpr bool ngt  = t2  > t1;
      constexpr bool gte  = t1 >= t2;
      constexpr bool ngte = t2 >= t1;

      CHECK_TRUE(eq);
      CHECK_FALSE(neq);
      CHECK_TRUE(lt);
      CHECK_FALSE(nlt);
      CHECK_TRUE(lte);
      CHECK_FALSE(nlte);
      CHECK_FALSE(gt);
      CHECK_TRUE(ngt);
      CHECK_FALSE(gte);
      CHECK_TRUE(ngte);
    }

#if ETL_USING_CPP14
    //*************************************************************************
    ETL_TYPEDEF(int32_t, arithmetic_type_t);

    constexpr arithmetic_type_t CreatePlus()
    {
      arithmetic_type_t value = 0;

      ++value;
      value++;
      value += 1;

      return value;
    }

    constexpr arithmetic_type_t CreateMinus()
    {
      arithmetic_type_t value = 0;

      --value;
      value--;
      value -= 1;

      return value;
    }
    
    TEST(test_arithmetic_constexpr)
    {
      constexpr arithmetic_type_t value_plus  = CreatePlus();
      constexpr arithmetic_type_t value_minus = CreateMinus();

      CHECK_EQUAL(3,  value_plus.get());
      CHECK_EQUAL(-3, value_minus.get());
    }
#endif
  };
}
