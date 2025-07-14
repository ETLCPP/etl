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

#if ETL_USING_CPP14 && !defined(ETL_CLOSURE_FORCE_CPP03_IMPLEMENTATION)

namespace
{
  SUITE(test_closure)
  {
    static constexpr int f1(int a1)
    {
      return a1 * 3;
    }

    static constexpr int f1_ref(const int& a1)
    {
      return a1 * 3;
    }

    static constexpr int f2(int a1, int a2)
    {
      return a1 * 3 + a2;
    }

    static constexpr int f3(int a1, int a2, int a3)
    {
      return a1 * 3 + a2 * a3;
    }

    static constexpr int f4(int a1, int a2, int a3, int a4)
    {
      return a1 * 3 + a2 * a3 + a4;
    }

    static constexpr int f5(int a1, int a2, int a3, int a4, int a5)
    {
      return a1 * 3 + a2 * a3 + a4 * a5;
    }

    static constexpr etl::delegate<int(int)> df1                     = etl::delegate<int(int)>::create<&f1>();
    static constexpr etl::delegate<int(int, int)> df2                = etl::delegate<int(int, int)>::create<&f2>();
    static constexpr etl::delegate<int(int, int, int)> df3           = etl::delegate<int(int, int, int)>::create<&f3>();
    static constexpr etl::delegate<int(int, int, int, int)> df4      = etl::delegate<int(int, int, int, int)>::create<&f4>();
    static constexpr etl::delegate<int(int, int, int, int, int)> df5 = etl::delegate<int(int, int, int, int, int)>::create<&f5>();

    static constexpr etl::delegate<int(const int&)> df1_ref = etl::delegate<int(const int&)>::create<&f1_ref>();

    //*************************************************************************
    constexpr etl::closure<int(int, int)> make_closure_2_args_bind(const etl::delegate<int(int, int)>& f, int arg0, int arg1)
    {
      etl::closure<int(int, int)> c(f, arg0, arg1);
      
      c.bind<0>(7);
      c.bind<1>(8);

      return c;
    }

    //*************************************************************************
    constexpr etl::closure<int(int, int)> make_closure_2_args_bind_all(const etl::delegate<int(int, int)>& f, int arg0, int arg1)
    {
      etl::closure<int(int, int)> c(f, arg0, arg1);
      
      c.bind(7, 8);

      return c;
    }

    //*************************************************************************
    TEST(test_1_arg)
    {
      static constexpr etl::closure<int(int)> c1(df1, 4);
      static constexpr int result = c1();
      CHECK_EQUAL(12, result);
    }

    //*************************************************************************
    TEST(test_1_arg_reference)
    {
      static constexpr int v1 = 4;
      static constexpr etl::closure<int(const int&)> c1_ref(df1_ref, v1);
      CHECK_EQUAL(12, c1_ref());
    }

    //*************************************************************************
    TEST(test_2_args)
    {
      static constexpr etl::closure<int(int, int)> c2(df2, 4, 3);
      static constexpr int result = c2();
      CHECK_EQUAL(15, result);
    }

    //*************************************************************************
    TEST(test_2_args_bind)
    {
      static constexpr etl::closure<int(int, int)> c2 = make_closure_2_args_bind(df2, 4, 3);
      static constexpr int result = c2();
      CHECK_EQUAL(29, result);
    }

    //*************************************************************************
    TEST(test_2_args_bind_all)
    {
      static constexpr etl::closure<int(int, int)> c2 = make_closure_2_args_bind_all(df2, 4, 3);
      static constexpr int result = c2();
      CHECK_EQUAL(29, result);
    }

    //*************************************************************************
    TEST(test_3_args)
    {
      static constexpr etl::closure<int(int, int, int)> c3(df3, 4, 3, 2);
      static constexpr int result = c3();
      CHECK_EQUAL(18, result);
    }

    //*************************************************************************
    TEST(test_4_args)
    {
      static constexpr etl::closure<int(int, int, int, int)> c4(df4, 4, 3, 2, 1);
      static constexpr int result = c4();
      CHECK_EQUAL(19, result);
    }

    //*************************************************************************
    TEST(test_5_args)
    {
      static constexpr etl::closure<int(int, int, int, int, int)> c5(df5, 4, 3, 2, 1, 5);
      static constexpr int result = c5();
      CHECK_EQUAL(23, result);
    }
  };
}

#endif
