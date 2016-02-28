/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

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

#include "../src/functional.h"

#include <list>
#include <vector>
#include <numeric>

namespace
{		
  SUITE(test_functional)
  {
    //*************************************************************************
    TEST(test_reference_wrapper)
    {
      int a = 0;
      etl::reference_wrapper<int> ra(a);

      ra = 1;
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
      std::list<int> compare = { 5, 6, 7, 8, 9 };
      std::vector<etl::reference_wrapper<int>> test_ref(test.begin(), test.end());
      
      std::iota(test_ref.begin(), test_ref.end(), 5);

      std::list<int>::const_iterator itest    = test.begin();
      std::list<int>::const_iterator icompare = compare.begin();
      std::vector<etl::reference_wrapper<int>>::const_iterator ivector = test_ref.begin();

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

      ra = 1;
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