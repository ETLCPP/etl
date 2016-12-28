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

#include "../src/instance_count.h"

#include <list>
#include <vector>
#include <numeric>

namespace
{		
  SUITE(test_instance_count)
  {
    //*************************************************************************
    TEST(test_count)
    {
      struct Test1 : public etl::instance_count<Test1>
      {};

      struct Test2 : public etl::instance_count<Test2>
      {};

      CHECK_EQUAL(0U, Test1::get_instance_count());
      CHECK_EQUAL(0U, Test2::get_instance_count());

      Test1 test1a;
      CHECK_EQUAL(1U, Test1::get_instance_count());
      CHECK_EQUAL(0U, Test2::get_instance_count());

      Test1 test1b;
      Test2 test2a;
      CHECK_EQUAL(2U, Test1::get_instance_count());
      CHECK_EQUAL(1U, Test2::get_instance_count());

      Test2* ptest2b = new Test2;
      CHECK_EQUAL(2U, Test1::get_instance_count());
      CHECK_EQUAL(2U, Test2::get_instance_count());

      delete ptest2b;
      CHECK_EQUAL(2U, Test1::get_instance_count());
      CHECK_EQUAL(1U, Test2::get_instance_count());
    }
  };
}
