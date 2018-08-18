/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2018 jwellbelove

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

#include "UnitTest++.h"

#undef min
#undef max

#include "etl/stl/alternate/functional.h"

namespace
{
  template <typename TCompare>
  bool compare(int a, int b)
  {
    return TCompare()(a, b);
  }
  
  struct test : std::binary_function<int, int, bool>
  {
    bool operator()(int a, int b) const
    {
      return a < b;
    }
  };

  SUITE(test_no_stl_functional)
  {
    //*************************************************************************
    TEST(test_less)
    {
      CHECK((compare<etlstd::less<int>>(1, 2)));
      CHECK(!(compare<etlstd::less<int>>(2, 1)));
      CHECK(!(compare<etlstd::less<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_greater)
    {
      CHECK(!(compare<etlstd::greater<int>>(1, 2)));
      CHECK((compare<etlstd::greater<int>>(2, 1)));
      CHECK(!(compare<etlstd::greater<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_equal_to)
    {
      CHECK((compare<etlstd::equal_to<int>>(1, 1)));
      CHECK(!(compare<etlstd::equal_to<int>>(1, 2)));
      CHECK(!(compare<etlstd::equal_to<int>>(2, 1)));
    }

    //*************************************************************************
    TEST(test_not_equal_to)
    {
      CHECK(!(compare<etlstd::not_equal_to<int>>(1, 1)));
      CHECK((compare<etlstd::not_equal_to<int>>(1, 2)));
      CHECK((compare<etlstd::not_equal_to<int>>(2, 1)));
    }

    //*************************************************************************
    TEST(test_bind1st)
    {
      CHECK((etlstd::bind1st(test(), 1)(2)));
      CHECK(!(etlstd::bind1st(test(), 2)(1)));
    }

    //*************************************************************************
    TEST(test_bind2nd)
    {
      CHECK(!(etlstd::bind2nd(test(), 1)(2)));
      CHECK((etlstd::bind2nd(test(), 2)(1)));
    }
  };
}
