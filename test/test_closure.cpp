/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG, John Wellbelove

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

#include "etl/closure.h"

namespace
{
  SUITE(test_closure)
  {
    int f1(int a1)
    {
      return a1 * 3;
    }

    int f1_ref(int& a1)
    {
      return a1 * 3;
    }

    int f2(int a1, int a2)
    {
      return a1 * 3 + a2;
    }

    int f3(int a1, int a2, int a3)
    {
      return a1 * 3 + a2 * a3;
    }

    int f4(int a1, int a2, int a3, int a4)
    {
      return a1 * 3 + a2 * a3 + a4;
    }

    int f5(int a1, int a2, int a3, int a4, int a5)
    {
      return a1 * 3 + a2 * a3 + a4 * a5;
    }

    etl::delegate<int(int)> df1 = etl::delegate<int(int)>::create<&f1>();
    etl::delegate<int(int, int)> df2 = etl::delegate<int(int, int)>::create<&f2>();
    etl::delegate<int(int, int, int)> df3 = etl::delegate<int(int, int, int)>::create<&f3>();
    etl::delegate<int(int, int, int, int)> df4 = etl::delegate<int(int, int, int, int)>::create<&f4>();
    etl::delegate<int(int, int, int, int, int)> df5 = etl::delegate<int(int, int, int, int, int)>::create<&f5>();

    etl::delegate<int(int&)> df1_ref = etl::delegate<int(int&)>::create<&f1_ref>();

    //*************************************************************************
    TEST(test_1_arg)
    {
      etl::closure<int(int)> c1(df1, 4);
      CHECK_EQUAL(12, c1());
    }

    //*************************************************************************
    TEST(test_1_arg_reference)
    {
      int v1 = 4;
      etl::closure<int(int&)> c1_ref(df1_ref, v1);
      CHECK_EQUAL(12, c1_ref());
      v1 = 5;
      CHECK_EQUAL(15, c1_ref());
    }

    //*************************************************************************
    TEST(test_1_arg_lambda)
    {
      auto l = [](int a){ return a + 11; };
      etl::delegate<int(int)> df1_lambda(l);

      etl::closure<int(int)> c1_lambda(df1_lambda, 5);
      CHECK_EQUAL(16, c1_lambda());
    }

    //*************************************************************************
    TEST(test_2_args)
    {
      etl::closure<int(int, int)> c2(df2, 4, 3);
      CHECK_EQUAL(15, c2());
    }

#if ETL_USING_CPP11 && !defined(ETL_CLOSURE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_2_args_bind)
    {
      etl::closure<int(int, int)> c2(df2, 4, 3);
      CHECK_EQUAL(15, c2());

      c2.bind<0>(7);
      c2.bind<1>(8);
      CHECK_EQUAL(29, c2());
    }

    //*************************************************************************
    TEST(test_2_args_bind_all)
    {
      etl::closure<int(int, int)> c2(df2, 4, 3);
      CHECK_EQUAL(15, c2());

      c2.bind(7, 8);
      CHECK_EQUAL(29, c2());
    }
#endif

    //*************************************************************************
    TEST(test_3_args)
    {
      etl::closure<int(int, int, int)> c3(df3, 4, 3, 2);
      CHECK_EQUAL(18, c3());
    }

    //*************************************************************************
    TEST(test_4_args)
    {
      etl::closure<int(int, int, int, int)> c4(df4, 4, 3, 2, 1);
      CHECK_EQUAL(19, c4());
    }

    //*************************************************************************
    TEST(test_5_args)
    {
      etl::closure<int(int, int, int, int, int)> c5(df5, 4, 3, 2, 1, 5);
      CHECK_EQUAL(23, c5());
    }

    //*************************************************************************
    TEST(test_bind_static_assert)
    {
      etl::closure<int(int, int)> c(df2, 1, 2);

      // Uncomment to generate static_assert errors.
      //c.bind(1);                // Argument count mismatch
      //c.bind(1, 2, 3);          // Argument count mismatch
      //c.bind(1, std::string()); // Argument is not convertible
    }
  };
}
