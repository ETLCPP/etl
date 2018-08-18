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

#include "etl/stl/alternate/utility.h"

namespace
{
  SUITE(test_no_stl_utility)
  {
    //*************************************************************************
    TEST(pair_default_construct)
    {
      etlstd::pair<int, double> p1;

      CHECK_EQUAL(int(), p1.first);
      CHECK_EQUAL(double(), p1.second);
    }

    //*************************************************************************
    TEST(test_pair_construct)
    {
      etlstd::pair<int, double> p1(1, 2.3);

      CHECK_EQUAL(1, p1.first);
      CHECK_EQUAL(2.3, p1.second);
    }

    //*************************************************************************
    TEST(test_pair_copy_construct)
    {
      etlstd::pair<int, double> p1(1, 2.3);
      etlstd::pair<int, double> p2(p1);

      CHECK_EQUAL(p1.first, p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_copy_construct_alternate)
    {
      etlstd::pair<char, float> p1(1, 2.3f);
      etlstd::pair<int, double> p2(p1);

      CHECK_EQUAL(p1.first, p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_make_pair)
    {
      etlstd::pair<int, double> p1(1, 2.3);
      etlstd::pair<int, double> p2;
      p2 = etlstd::make_pair(1, 2.3);

      CHECK_EQUAL(p1.first, p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_swap_member)
    {
      etlstd::pair<int, double> p1(1, 2.3);
      etlstd::pair<int, double> p2(2, 3.4);

      p1.swap(p2);

      CHECK_EQUAL(2, p1.first);
      CHECK_EQUAL(3.4, p1.second);

      CHECK_EQUAL(1, p2.first);
      CHECK_EQUAL(2.3, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_swap_global)
    {
      etlstd::pair<int, double> p1(1, 2.3);
      etlstd::pair<int, double> p2(2, 3.4);

      etlstd::swap(p1, p2);

      CHECK_EQUAL(2, p1.first);
      CHECK_EQUAL(3.4, p1.second);

      CHECK_EQUAL(1, p2.first);
      CHECK_EQUAL(2.3, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_conditional)
    {
      etlstd::pair<int, double> p1(1, 2.3);
      etlstd::pair<int, double> p2(1, 2.3);
      etlstd::pair<int, double> p3(2, 3.4);

      CHECK(p1 == p2);
      CHECK(!(p1 == p3));
      CHECK(p1 != p3);
      CHECK(!(p1 != p2));
      CHECK(p1 <= p2);
      CHECK(p1 <= p3);
      CHECK(!(p1 < p2));
      CHECK(p1 < p3);
      CHECK(!(p3 < p1));
      CHECK(p1 >= p2);
      CHECK(!(p1 >= p3));
      CHECK(!(p1 > p2));
      CHECK(!(p1 > p3));
      CHECK(p3 > p1);
    }
  };
}
