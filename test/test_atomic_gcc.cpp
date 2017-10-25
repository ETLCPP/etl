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

#include "UnitTest++.h"

#include "platform.h"

#include "atomic/atomic_gcc.h"

#include <atomic>

namespace
{
  SUITE(test_atomic_gcc)
  {
    //=========================================================================
    TEST(test_atomic_compare_exchange_weak_fail)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      int actual = 1U;

      compare = actual;
      test    = actual;

      int compare_expected = 2U;
      int test_expected    = 2U;
      int desired  = 3U;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result    = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result,   test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(),   test.load());
    }

        //=========================================================================
    TEST(test_atomic_compare_exchange_weak_pass)
    {
      std::atomic<int> compare;
      etl::atomic<int> test;

      int actual = 1U;

      compare = actual;
      test    = actual;

      int compare_expected = actual;
      int test_expected    = actual;
      int desired  = 3U;

      bool compare_result = compare.compare_exchange_weak(compare_expected, desired);
      bool test_result    = test.compare_exchange_weak(test_expected, desired);

      CHECK_EQUAL(compare_result,   test_result);
      CHECK_EQUAL(compare_expected, test_expected);
      CHECK_EQUAL(compare.load(),   test.load());
    }
  };
}
