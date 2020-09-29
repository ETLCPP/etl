/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "UnitTest++/UnitTest++.h"

#include "etl/functional.h"

#include <list>
#include <vector>
#include <numeric>

namespace
{
  template <typename TCompare>
  bool compare(int a, int b)
  {
    return TCompare()(a, b);
  }

  struct test : etl::binary_function<int, int, bool>
  {
    bool operator()(int a, int b) const
    {
      return a < b;
    }
  };

  SUITE(test_functional)
  {
    //*************************************************************************
    TEST(test_less)
    {
      CHECK((compare<etl::less<int>>(1, 2)));
      CHECK(!(compare<etl::less<int>>(2, 1)));
      CHECK(!(compare<etl::less<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_greater)
    {
      CHECK(!(compare<etl::greater<int>>(1, 2)));
      CHECK((compare<etl::greater<int>>(2, 1)));
      CHECK(!(compare<etl::greater<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_equal_to)
    {
      CHECK((compare<etl::equal_to<int>>(1, 1)));
      CHECK(!(compare<etl::equal_to<int>>(1, 2)));
      CHECK(!(compare<etl::equal_to<int>>(2, 1)));
    }

    //*************************************************************************
    TEST(test_not_equal_to)
    {
      CHECK(!(compare<etl::not_equal_to<int>>(1, 1)));
      CHECK((compare<etl::not_equal_to<int>>(1, 2)));
      CHECK((compare<etl::not_equal_to<int>>(2, 1)));
    }

    //*************************************************************************
    TEST(test_bind1st)
    {
      CHECK((etl::bind1st(test(), 1)(2)));
      CHECK(!(etl::bind1st(test(), 2)(1)));
    }

    //*************************************************************************
    TEST(test_bind2nd)
    {
      CHECK(!(etl::bind2nd(test(), 1)(2)));
      CHECK((etl::bind2nd(test(), 2)(1)));
    }

    //*************************************************************************
    TEST(test_reference_wrapper)
    {
      int a = 0;
      etl::reference_wrapper<int> ra(a);

      ra.get() = 1;
      CHECK_EQUAL(1, a);
      CHECK_EQUAL(1, ra);

      a = 2;
      CHECK_EQUAL(2, a);
      CHECK_EQUAL(2, ra);
    }

    //*************************************************************************
    TEST(test_reference_wrapper_container)
    {
      std::list<int> test    = { 0, 1, 2, 3, 4 };
      std::list<int> compare = { 0, 1, 2, 3, 4 };
      std::vector<etl::reference_wrapper<int>> test_ref(test.begin(), test.end());

      std::list<int>::const_iterator itest    = test.begin();
      std::list<int>::const_iterator icompare = compare.begin();

      while (icompare != compare.end())
      {
        CHECK_EQUAL(*icompare, *itest);
        ++itest;
        ++icompare;
      }
    }

    //*************************************************************************
    TEST(test_ref)
    {
      int a = 0;
      etl::reference_wrapper<int> ra = etl::ref(a);

      ra.get() = 1;
      CHECK_EQUAL(1, a);
      CHECK_EQUAL(1, ra);
    }

    //*************************************************************************
    TEST(test_cref)
    {
      int a = 0;
      etl::reference_wrapper<const int> ra = etl::cref(a);

      a = 1;
      CHECK_EQUAL(1, a);
      CHECK_EQUAL(1, ra);
    }
  };
}
