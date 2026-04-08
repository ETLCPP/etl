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
#include "etl/inplace_function.h"

#include <stdexcept>

namespace
{
  SUITE(test_closure_with_inplace_function)
  {
    int f1(int a1)
    {
      return a1 * 3;
    }

    int f1_throwing(int)
    {
      throw std::runtime_error("throwing function");
    }

    void f1_void(int) {}

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

    using f1_type      = int(int);
    using f1_ref_type  = int(int&);
    using f1_void_type = void(int);
    using f2_type      = int(int, int);
    using f3_type      = int(int, int, int);
    using f4_type      = int(int, int, int, int);
    using f5_type      = int(int, int, int, int, int);

    using ipf1_type      = etl::inplace_function<f1_type>;
    using ipf1_ref_type  = etl::inplace_function<f1_ref_type>;
    using ipf1_void_type = etl::inplace_function<f1_void_type>;
    using ipf2_type      = etl::inplace_function<f2_type>;
    using ipf3_type      = etl::inplace_function<f3_type>;
    using ipf4_type      = etl::inplace_function<f4_type>;
    using ipf5_type      = etl::inplace_function<f5_type>;

    ipf1_type      ipf1          = ipf1_type::create<&f1>();
    ipf1_type      ipf1_throwing = ipf1_type::create<&f1_throwing>();
    ipf1_ref_type  ipf1_ref      = ipf1_ref_type::create<&f1_ref>();
    ipf1_void_type ipf1_void     = ipf1_void_type::create<&f1_void>();
    ipf2_type      ipf2          = ipf2_type::create<&f2>();
    ipf3_type      ipf3          = ipf3_type::create<&f3>();
    ipf4_type      ipf4          = ipf4_type::create<&f4>();
    ipf5_type      ipf5          = ipf5_type::create<&f5>();

    //*************************************************************************
    TEST(test_1_arg)
    {
      etl::closure<f1_type, ipf1_type> c1(ipf1, 4);
      CHECK_EQUAL(12, c1());
    }

    //*************************************************************************
    TEST(test_1_arg_reference)
    {
      int                     v1 = 4;
      etl::closure<f1_ref_type, ipf1_ref_type> c1_ref(ipf1_ref, v1);
      CHECK_EQUAL(12, c1_ref());
      v1 = 5;
      CHECK_EQUAL(15, c1_ref());
    }

    //*************************************************************************
    TEST(test_1_arg_lambda)
    {
      auto l = [](int a)
      {
        return a + 11;
      };
      ipf1_type ipf1_lambda(l);

      etl::closure<f1_type, ipf1_type> c1_lambda(ipf1_lambda, 5);
      CHECK_EQUAL(16, c1_lambda());
    }

    //*************************************************************************
    TEST(test_throwing)
    {
      etl::closure<f1_type, ipf1_type> c1(ipf1_throwing, 4);
      CHECK_THROW(c1(), std::runtime_error);
    }

    //*************************************************************************
    TEST(test_void)
    {
      etl::closure<f1_void_type, ipf1_void_type> c1(ipf1_void, 4);
      c1();
    }

    //*************************************************************************
    TEST(test_2_args)
    {
      etl::closure<f2_type, ipf2_type> c2(ipf2, 4, 3);
      CHECK_EQUAL(15, c2());
    }

#if ETL_USING_CPP11 && !defined(ETL_CLOSURE_FORCE_CPP03_IMPLEMENTATION)
    //*************************************************************************
    TEST(test_2_args_bind)
    {
      etl::closure<f2_type, ipf2_type> c2(ipf2, 4, 3);
      CHECK_EQUAL(15, c2());

      c2.bind<0>(7);
      c2.bind<1>(8);
      CHECK_EQUAL(29, c2());
    }

    //*************************************************************************
    TEST(test_2_args_bind_all)
    {
      etl::closure<f2_type, ipf2_type> c2(ipf2, 4, 3);
      CHECK_EQUAL(15, c2());

      c2.bind(7, 8);
      CHECK_EQUAL(29, c2());
    }
#endif

    //*************************************************************************
    TEST(test_3_args)
    {
      etl::closure<f3_type, ipf3_type> c3(ipf3, 4, 3, 2);
      CHECK_EQUAL(18, c3());
    }

    //*************************************************************************
    TEST(test_4_args)
    {
      etl::closure<f4_type, ipf4_type> c4(ipf4, 4, 3, 2, 1);
      CHECK_EQUAL(19, c4());
    }

    //*************************************************************************
    TEST(test_5_args)
    {
      etl::closure<f5_type, ipf5_type> c5(ipf5, 4, 3, 2, 1, 5);
      CHECK_EQUAL(23, c5());
    }

    //*************************************************************************
    TEST(test_bind_static_assert)
    {
      etl::closure<f2_type, ipf2_type> c(ipf2, 1, 2);

      // Uncomment to generate static_assert errors.
      // c.bind(1);                // Argument count mismatch
      // c.bind(1, 2, 3);          // Argument count mismatch
      // c.bind(1, std::string()); // Argument is not convertible
    }
  }
} // namespace
