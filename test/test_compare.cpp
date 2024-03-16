/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

  struct Object
  {
    int a;
    int b;
  };

  Object ta = { 1, 2 };
  Object tb = { 2, 3 };

  //***********************************
  bool constexpr operator <(const Object& lhs, const Object& rhs)
  {
    return (lhs.a + lhs.b) < (rhs.a + rhs.b);
  }

  //***********************************
  bool constexpr operator >(const Object& lhs, const Object& rhs)
  {
    return (lhs.a + lhs.b) > (rhs.a + rhs.b);
  }

  //***********************************
  bool constexpr operator <=(const Object& lhs, const Object& rhs)
  {
    return (lhs.a + lhs.b) <= (rhs.a + rhs.b);
  }

  //***********************************
  bool constexpr operator >=(const Object& lhs, const Object& rhs)
  {
    return (lhs.a + lhs.b) >= (rhs.a + rhs.b);
  }

  //***********************************
  bool constexpr operator ==(const Object& lhs, const Object& rhs)
  {
    return (lhs.a + lhs.b) == (rhs.a + rhs.b);
  }

  //***********************************
  bool constexpr operator !=(const Object& lhs, const Object& rhs)
  {
    return (lhs.a + lhs.b) != (rhs.a + rhs.b);
  }

  //***********************************
  struct LessTest
  {
    bool constexpr operator()(const Object& lhs, const Object& rhs) const
    {
      return (lhs.a + lhs.b) < (rhs.a + rhs.b);
    }
  };

  typedef etl::compare<Object, LessTest> CompareTest;

  SUITE(test_compare)
  {
    //*************************************************************************
    TEST(test_lt)
    {
      CHECK_EQUAL(a < b, (CompareInt::lt(a, b)));
      CHECK_EQUAL(b < a, (CompareInt::lt(b, a)));
      CHECK_EQUAL(a < a, (CompareInt::lt(a, a)));
      CHECK_EQUAL(ta < tb, (CompareTest::lt(ta, tb)));
      CHECK_EQUAL(tb < ta, (CompareTest::lt(tb, ta)));
      CHECK_EQUAL(ta < ta, (CompareTest::lt(ta, ta)));
    }

    //*************************************************************************
    TEST(test_lte)
    {
      CHECK_EQUAL(a <= b, (CompareInt::lte(a, b)));
      CHECK_EQUAL(b <= a, (CompareInt::lte(b, a)));
      CHECK_EQUAL(a <= a, (CompareInt::lte(a, a)));
      CHECK_EQUAL(ta <= tb, (CompareTest::lte(ta, tb)));
      CHECK_EQUAL(tb <= ta, (CompareTest::lte(tb, ta)));
      CHECK_EQUAL(ta <= ta, (CompareTest::lte(ta, ta)));
    }

    //*************************************************************************
    TEST(test_gt)
    {
      CHECK_EQUAL(a > b, (CompareInt::gt(a, b)));
      CHECK_EQUAL(b > a, (CompareInt::gt(b, a)));
      CHECK_EQUAL(a > a, (CompareInt::gt(a, a)));
      CHECK_EQUAL(ta > tb, (CompareTest::gt(ta, tb)));
      CHECK_EQUAL(tb > ta, (CompareTest::gt(tb, ta)));
      CHECK_EQUAL(ta > ta, (CompareTest::gt(ta, ta)));
    }

    //*************************************************************************
    TEST(test_gte)
    {
      CHECK_EQUAL(a >= b, (CompareInt::gte(a, b)));
      CHECK_EQUAL(b >= a, (CompareInt::gte(b, a)));
      CHECK_EQUAL(a >= a, (CompareInt::gte(a, a)));
      CHECK_EQUAL(ta >= tb, (CompareTest::gte(ta, tb)));
      CHECK_EQUAL(tb >= ta, (CompareTest::gte(tb, ta)));
      CHECK_EQUAL(ta >= ta, (CompareTest::gte(ta, ta)));
    }

    //*************************************************************************
    TEST(test_eq)
    {
      CHECK_EQUAL(a == b, (CompareInt::eq(a, b)));
      CHECK_EQUAL(b == a, (CompareInt::eq(b, a)));
      CHECK_EQUAL(a == a, (CompareInt::eq(a, a)));
      CHECK_EQUAL(ta == tb, (CompareTest::eq(ta, tb)));
      CHECK_EQUAL(tb == ta, (CompareTest::eq(tb, ta)));
      CHECK_EQUAL(ta == ta, (CompareTest::eq(ta, ta)));
    }

    //*************************************************************************
    TEST(test_ne)
    {
      CHECK_EQUAL(a != b, (CompareInt::ne(a, b)));
      CHECK_EQUAL(b != a, (CompareInt::ne(b, a)));
      CHECK_EQUAL(a != a, (CompareInt::ne(a, a)));
      CHECK_EQUAL(ta != tb, (CompareTest::ne(ta, tb)));
      CHECK_EQUAL(tb != ta, (CompareTest::ne(tb, ta)));
      CHECK_EQUAL(ta != ta, (CompareTest::ne(ta, ta)));
    }

    //*************************************************************************
    TEST(test_cmp)
    {
      CHECK_EQUAL(CompareInt::Less, (CompareInt::cmp(2, 4)));
      CHECK_EQUAL(CompareInt::Greater, (CompareInt::cmp(4, 2)));
      CHECK_EQUAL(CompareInt::Equal, (CompareInt::cmp(0, 0)));
      CHECK_EQUAL(CompareTest::Less, (CompareTest::cmp(Object{ 0, 1 }, Object{ 2, 4 })));
      CHECK_EQUAL(CompareTest::Greater, (CompareTest::cmp(Object{ 2, 4 }, Object{ 0, 1 })));
      CHECK_EQUAL(CompareTest::Equal, (CompareTest::cmp(Object{ 2, 4 }, Object{ 2, 4 })));
    }

    //*************************************************************************
    TEST(test_constexpr_cmp)
    {
      constexpr int cmp_less    = CompareInt::cmp(2, 4);
      constexpr int cmp_greater = CompareInt::cmp(4, 2);
      constexpr int cmp_equal   = CompareInt::cmp(0, 0);

      constexpr int cmp_test_less    = CompareTest::cmp(Object{ 0, 1 }, Object{ 2, 4 });
      constexpr int cmp_test_greater = CompareTest::cmp(Object{ 2, 4 }, Object{ 0, 1 });
      constexpr int cmp_test_equal   = CompareTest::cmp(Object{ 2, 4 }, Object{ 2, 4 });

      CHECK_EQUAL(CompareInt::Less,    cmp_less);
      CHECK_EQUAL(CompareInt::Greater, cmp_greater);
      CHECK_EQUAL(CompareInt::Equal,   cmp_equal);

      CHECK_EQUAL(CompareInt::Less,    cmp_test_less);
      CHECK_EQUAL(CompareInt::Greater, cmp_test_greater);
      CHECK_EQUAL(CompareInt::Equal,   cmp_test_equal);
    }
  };
}
