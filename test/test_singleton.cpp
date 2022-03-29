/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 jwellbelove

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

#include "etl/singleton.h"

#include <string>
#include <type_traits>

class Test_Class
{
public:

  Test_Class(int i_, std::string text_)
    : i(i_)
    , text(text_)
  {
  }

  void Increment()
  {
    ++i;
    text += "*";
  }

  int i;
  std::string text;
};

using Test_Singleton = etl::singleton<Test_Class>;

namespace
{
  SUITE(test_singleton)
  {
    //*************************************************************************
    TEST(test1)
    {
      CHECK((std::is_same_v<Test_Singleton::type, Test_Class>));

      CHECK(!Test_Singleton::is_valid());
      CHECK_THROW(Test_Singleton::instance(), etl::singleton_not_created);

      Test_Singleton::create(1, "Start:");
      CHECK(Test_Singleton::is_valid());

      Test_Class& ts = Test_Singleton::instance();

      CHECK_EQUAL(1, ts.i);
      CHECK_EQUAL("Start:", ts.text);

      ts.Increment();

      CHECK_EQUAL(2, ts.i);
      CHECK_EQUAL("Start:*", ts.text);

      Test_Class* pts = &Test_Singleton::instance();

      CHECK_EQUAL(2, ts.i);
      CHECK_EQUAL(2, pts->i);
      CHECK_EQUAL("Start:*", ts.text);
      CHECK_EQUAL("Start:*", pts->text);

      pts->Increment();

      CHECK_EQUAL(3, ts.i);
      CHECK_EQUAL(3, pts->i);
      CHECK_EQUAL("Start:**", ts.text);
      CHECK_EQUAL("Start:**", pts->text);

      Test_Singleton::destroy();
      CHECK(!Test_Singleton::is_valid());
      CHECK_THROW(Test_Singleton::instance(), etl::singleton_not_created);
    }
  }
}

