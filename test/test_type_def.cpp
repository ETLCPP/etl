/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2016 jwellbelove

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

#include <UnitTest++/UnitTest++.h>

#include <stdint.h>

#include "../src/type_def.h"

namespace
{
  SUITE(test_type_def)
  {
    //=========================================================================
    TEST(test_implicit)
    {
      class __type1_t__;
      typedef etl::type_def<__type1_t__, uint32_t> type1_t;

      class __type2_t__;
      typedef etl::type_def<__type2_t__, uint32_t> type2_t;

      type1_t t1 = type1_t(1);
      type2_t t2 = type2_t(1);

      uint32_t i1 = t1;
      uint32_t i2 = t2;

      CHECK_EQUAL(i1, i2);
    }

    //=========================================================================
    TEST(test_get)
    {
      class __type1_t__;
      typedef etl::type_def<__type1_t__, uint32_t> type1_t;

      class __type2_t__;
      typedef etl::type_def<__type2_t__, uint32_t> type2_t;

      type1_t t1(1);
      type2_t t2(1);

      CHECK_EQUAL(t1.get(), t2.get());
    }

    //=========================================================================
    TEST(test_operators)
    {
      class __type_t__;
      typedef etl::type_def<__type_t__, uint32_t> type_t;

      uint32_t i = 0x5A3D;
      type_t t(0x5A3D);

      CHECK_EQUAL(++i, ++t);
      CHECK_EQUAL(i++, t++);
      CHECK_EQUAL(--i, --t);
      CHECK_EQUAL(i--, t--);
      CHECK_EQUAL(i += 2, t += 2);
      CHECK_EQUAL(i += 2, t += type_t(2));
      CHECK_EQUAL(i -= 2, t -= 2);
      CHECK_EQUAL(i -= 2, t -= type_t(2));
      CHECK_EQUAL(i *= 2, t *= 2);
      CHECK_EQUAL(i *= 2, t *= type_t(2));
      CHECK_EQUAL(i /= 2, t /= 2);
      CHECK_EQUAL(i /= 2, t /= type_t(2));
      CHECK_EQUAL(i &= 0xFF00, t &= 0xFF00);
      CHECK_EQUAL(i &= 0xFF00, t &= type_t(0xFF00));
      CHECK_EQUAL(i |= 0x003D, t |= 0x003D);
      CHECK_EQUAL(i |= 0x003D, t |= type_t(0x003D));
      CHECK_EQUAL(i ^= 0xAA55, t ^= 0xAA55);
      CHECK_EQUAL(i ^= 0xAA55, t ^= type_t(0xAA55));
      CHECK_EQUAL(i <<= 2, t <<= 2);
      CHECK_EQUAL(i >>= 2, t >>= 2);
      CHECK_EQUAL(i %= 23, t %= 23);

      t = type_t(0x1234);
      CHECK_EQUAL(0x1234, t);
    }

    //=========================================================================
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
  };
}
