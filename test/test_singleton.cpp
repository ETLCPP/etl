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

#include "../singleton.h"

class Test_Class
{
public:

  Test_Class()
    : i(0)
  {}

  void Increment()
  {
    ++i;
  }
  
  int i;
};

typedef etl::singleton<Test_Class> Test_Singleton;

namespace
{
  SUITE(test_singleton)
  {
    //*************************************************************************
    TEST(test1)
    {
      Test_Class& ts = Test_Singleton::instance();

      CHECK_EQUAL(0, ts.i);

      ts.Increment();

      CHECK_EQUAL(1, ts.i);

      Test_Class* pts = &Test_Singleton::instance();

      CHECK_EQUAL(1, ts.i);
      CHECK_EQUAL(1, pts->i);

      pts->Increment();

      CHECK_EQUAL(2, ts.i);
      CHECK_EQUAL(2, pts->i);
    }
  }
}

