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

#include "../src/utility.h"

namespace
{
  bool nonConstCalled;
  bool constCalled;

  void TestText(std::string&)
  {
    nonConstCalled = true;
  }

  void TestText(const std::string&)
  {
    constCalled = true;
  }
}

namespace
{
  SUITE(test_utility)
  {
    //=========================================================================
    TEST(test_exchange)
    {
      int a = 1;
      int b = 2;
      int c = etl::exchange(a, b); // c = a, a = b

      CHECK_EQUAL(2, a);
      CHECK_EQUAL(2, b);
      CHECK_EQUAL(1, c);
    }

    //=========================================================================
    TEST(test_exchange_const)
    {
      int a = 1;
      const int b = 2;
      int c = etl::exchange(a, b); // c = a, a = b

      CHECK_EQUAL(2, a);
      CHECK_EQUAL(2, b);
      CHECK_EQUAL(1, c);
    }

    //=========================================================================
    TEST(test_as_const)
    {
      std::string text = "Hello World!";
      
      nonConstCalled = false;
      constCalled    = false;
      
      TestText(text);

      CHECK(nonConstCalled);
      CHECK(!constCalled);

      nonConstCalled = false;
      constCalled = false;

      TestText(etl::as_const(text));

      CHECK(!nonConstCalled);
      CHECK(constCalled);
    }
  };
}
