/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#include "etl/compare.h"

namespace
{
  int a = 1;
  int b = 2;

  typedef etl::compare<int> CompareInt;

  struct Test
  {
    int a;
    int b;
  };

  Test ta = { 1, 2 };
  Test tb = { 2, 3 };

  //***********************************
  bool operator <(const Test& lhs, const Test& rhs)
  {
    return (lhs.a + lhs.b) < (rhs.a + rhs.b);
  }

  //***********************************
  bool operator >(const Test& lhs, const Test& rhs)
  {
    return (lhs.a + lhs.b) > (rhs.a + rhs.b);
  }

  //***********************************
  bool operator <=(const Test& lhs, const Test& rhs)
  {
    return (lhs.a + lhs.b) <= (rhs.a + rhs.b);
  }

  //***********************************
  bool operator >=(const Test& lhs, const Test& rhs)
  {
    return (lhs.a + lhs.b) >= (rhs.a + rhs.b);
  }

  //***********************************
  struct LessTest
  {
    bool operator()(const Test& lhs, const Test& rhs) const
    {
      return (lhs.a + lhs.b) < (rhs.a + rhs.b);
    }
  };

  typedef etl::compare<Test, LessTest> CompareTest;

  SUITE(test_compare)
  {
    //*************************************************************************
    TEST(test_lt)
    { 
      CHECK_EQUAL(a < b,   (CompareInt::lt(a, b)));
      CHECK_EQUAL(b < a,   (CompareInt::lt(b, a)));
      CHECK_EQUAL(a < a,   (CompareInt::lt(a, a)));
      CHECK_EQUAL(ta < tb, (CompareTest::lt(ta, tb)));
      CHECK_EQUAL(tb < ta, (CompareTest::lt(tb, ta)));
      CHECK_EQUAL(ta < ta, (CompareTest::lt(ta, ta)));
    }

    //*************************************************************************
    TEST(test_lte)
    {
      CHECK_EQUAL(a <= b,   (CompareInt::lte(a, b)));
      CHECK_EQUAL(b <= a,   (CompareInt::lte(b, a)));
      CHECK_EQUAL(a <= a,   (CompareInt::lte(a, a)));
      CHECK_EQUAL(ta <= tb, (CompareTest::lte(ta, tb)));
      CHECK_EQUAL(tb <= ta, (CompareTest::lte(tb, ta)));
      CHECK_EQUAL(ta <= ta, (CompareTest::lte(ta, ta)));
    }

    //*************************************************************************
    TEST(test_gt)
    {
      CHECK_EQUAL(a > b,   (CompareInt::gt(a, b)));
      CHECK_EQUAL(b > a,   (CompareInt::gt(b, a)));
      CHECK_EQUAL(a > a,   (CompareInt::gt(a, a)));
      CHECK_EQUAL(ta > tb, (CompareTest::gt(ta, tb)));
      CHECK_EQUAL(tb > ta, (CompareTest::gt(tb, ta)));
      CHECK_EQUAL(ta > ta, (CompareTest::gt(ta, ta)));
    }

    //*************************************************************************
    TEST(test_gte)
    {
      CHECK_EQUAL(a >= b,   (CompareInt::gte(a, b)));
      CHECK_EQUAL(b >= a,   (CompareInt::gte(b, a)));
      CHECK_EQUAL(a >= a,   (CompareInt::gte(a, a)));
      CHECK_EQUAL(ta >= tb, (CompareTest::gte(ta, tb)));
      CHECK_EQUAL(tb >= ta, (CompareTest::gte(tb, ta)));
      CHECK_EQUAL(ta >= ta, (CompareTest::gte(ta, ta)));
    }
  };
}
