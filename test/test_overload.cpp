/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2015 John Wellbelove

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

#include "etl/overload.h"

#if ETL_USING_CPP11

#include <iostream>

namespace
{
  struct Result
  {
    Result()
      : bi(false)
      , bd(false)
      , bs(false)
    {
    }

    void clear()
    {
      bi = false;
      bd = false;
      bs = false;
    }

    bool bi;
    bool bd;
    bool bs;
  };

  Result result;

  struct Visitor
  {
    void operator ()(int i) { result.bi = true; }
    void operator ()(double d) { result.bd = true; }
    void operator ()(const std::string& s) { result.bs = true; }
  };

  template <typename T, typename TOverload>
  void Function(T value, TOverload&& ol)
  {
    ol(value);
  }

  SUITE(test_overload)
  {
    //*************************************************************************
    TEST(test_overload_lambdas_make_overload)
    {
      auto overload = etl::make_overload([](int i) { result.bi = true; },
                                         [](double d) { result.bd = true; },
                                         [](const std::string& s) { result.bs = true; });

      result.clear();
      Function(int(1), overload);
      CHECK(result.bi == true);
      CHECK(result.bd == false);
      CHECK(result.bs == false);

      result.clear();
      Function(double(2.0), overload);
      CHECK(result.bi == false);
      CHECK(result.bd == true);
      CHECK(result.bs == false);

      result.clear();
      Function(std::string("3"), overload);
      CHECK(result.bi == false);
      CHECK(result.bd == false);
      CHECK(result.bs == true);
    }

#if ETL_USING_CPP_17
    //*************************************************************************
    TEST(test_overload_lambdas_initializer_list)
    {
      result.clear();
      Function(int(1), etl::overload
                       {
                         [](int i) { result.bi = true; },
                         [](double d) { result.bd = true; },
                         [](const std::string& s) { result.bs = true; }
                       });
      CHECK(result.bi == true);
      CHECK(result.bd == false);
      CHECK(result.bs == false);

      result.clear();
      Function(double(2.0), etl::overload
                            {
                              [](int i) { result.bi = true; },
                              [](double d) { result.bd = true; },
                              [](const std::string& s) { result.bs = true; }
                            });
      CHECK(result.bi == false);
      CHECK(result.bd == true);
      CHECK(result.bs == false);

      result.clear();
      Function(std::string("3"), etl::overload
                                 {
                                   [](int i) { result.bi = true; },
                                   [](double d) { result.bd = true; },
                                   [](const std::string& s) { result.bs = true; }
                                 });
      CHECK(result.bi == false);
      CHECK(result.bd == false);
      CHECK(result.bs == true);
    }
#endif

    //*************************************************************************
    TEST(test_visitor_overload)
    {
      auto overload = etl::make_overload(Visitor());

      result.clear();
      Function(int(1), overload);
      CHECK(result.bi == true);
      CHECK(result.bd == false);
      CHECK(result.bs == false);

      result.clear();
      Function(double(2.0), overload);
      CHECK(result.bi == false);
      CHECK(result.bd == true);
      CHECK(result.bs == false);

      result.clear();
      Function(std::string("3"), overload);
      CHECK(result.bi == false);
      CHECK(result.bd == false);
      CHECK(result.bs == true);
    }
  };
}

#endif
