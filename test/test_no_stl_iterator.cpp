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

#include "etl/stl/alternate/iterator.h"

#include <algorithm>
#include <vector>
#include <list>

#include "no_stl_test_iterators.h"

namespace
{
  const size_t SIZE = 10;

  int dataA[SIZE] = { 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };

  SUITE(test_no_stl_iterator)
  {
    //*************************************************************************
    TEST(distance_non_random)
    {
      ptrdiff_t d = etlstd::distance(non_random_iterator<int>(&dataA[0]), non_random_iterator<int>(&dataA[SIZE]));

      CHECK_EQUAL(SIZE, d);
    }

    //*************************************************************************
    TEST(distance_random)
    {
      ptrdiff_t d = etlstd::distance(random_iterator<int>(&dataA[0]), random_iterator<int>(&dataA[SIZE]));

      CHECK_EQUAL(SIZE, d);
    }

    //*************************************************************************
    TEST(advance_non_random)
    {
      int* itr1 = std::begin(dataA);
      non_random_iterator<int> itr2 = std::begin(dataA);
      
      std::advance(itr1, 4);
      etlstd::advance(itr2, 4);
      CHECK_EQUAL(*itr1, *itr2);

      std::advance(itr1, -3);
      etlstd::advance(itr2, -3);
      CHECK_EQUAL(*itr1, *itr2);
    }

    //*************************************************************************
    TEST(advance_random)
    {
      int* itr1 = std::begin(dataA);
      random_iterator<int> itr2 = std::begin(dataA);

      std::advance(itr1, 4);
      etlstd::advance(itr2, 4);
      CHECK_EQUAL(*itr1, *itr2);

      std::advance(itr1, -3);
      etlstd::advance(itr2, -3);
      CHECK_EQUAL(*itr1, *itr2);
    }
  };
}
