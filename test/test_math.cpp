/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

Copyright(c) 2014 jwellbelove

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

#include "../log.h"
#include "../pow.h"

namespace
{		
  SUITE(TestMaths)
  {
    //*************************************************************************
    TEST(TestLog0_BASE)
    {
      int actual;

      actual = etl::log<0, 2>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log<0, 10>::value;
      CHECK_EQUAL(0, actual);
    }

    //*************************************************************************
    TEST(TestLog1_BASE)
    {
      int actual;

      actual = etl::log<1, 2>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log<1, 10>::value;
      CHECK_EQUAL(0, actual);
    }

    //*************************************************************************
    TEST(TestLog10_BASE)
    {
      int actual;

      actual = etl::log<10, 2>::value;
      CHECK_EQUAL(3, actual);

      actual = etl::log<10, 10>::value;
      CHECK_EQUAL(1, actual);
    }

    //*************************************************************************
    TEST(TestLog100_BASE)
    {
      int actual;

      actual = etl::log<100, 2>::value;
      CHECK_EQUAL(6, actual);

      actual = etl::log<100, 10>::value;
      CHECK_EQUAL(2, actual);
    }

    //*************************************************************************
    TEST(TestLog2)
    {
      int actual;

      actual = etl::log2<0>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log2<1>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log2<10>::value;
      CHECK_EQUAL(3, actual);

      actual = etl::log2<100>::value;
      CHECK_EQUAL(6, actual);
    }

    //*************************************************************************
    TEST(TestLog10)
    {
      int actual;

      actual = etl::log10<0>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log10<1>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log10<10>::value;
      CHECK_EQUAL(1, actual);

      actual = etl::log10<100>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::log10<100>::value;
      CHECK_EQUAL(2, actual);
    }

    //*************************************************************************
    TEST(TestPow)
    {
      int actual;

      // 2^1
      actual = etl::pow<2, 1>::value;
      CHECK_EQUAL(2, actual);

      // 3^2
      actual = etl::pow<3, 2>::value;
      CHECK_EQUAL(9, actual);

      // 4^3
      actual = etl::pow<4, 3>::value;
      CHECK_EQUAL(64, actual);

      // 5^4
      actual = etl::pow<5, 4>::value;
      CHECK_EQUAL(625, actual);

      // 6^5
      actual = etl::pow<6, 5>::value;
      CHECK_EQUAL(7776, actual);

      // 7^6
      actual = etl::pow<7, 6>::value;
      CHECK_EQUAL(117649, actual);

      // 8^7
      actual = etl::pow<8, 7>::value;
      CHECK_EQUAL(2097152, actual);

      // 9^8
      actual = etl::pow<9, 8>::value;
      CHECK_EQUAL(43046721, actual);

      // 10^9
      actual = etl::pow<10, 9>::value;
      CHECK_EQUAL(1000000000, actual);
    }
  };
}