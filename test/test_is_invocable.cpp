/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#include <type_traits>
#include <string>

#include "etl/type_traits.h"
#include "etl/invoke.h"

namespace
{
  // Free functions
  int f0() { return 0; }
  long f1(double, const char*) { return 0; }
  void fv(int) { }

  // Add a noexcept free function
  int fn_noexcept(int x) noexcept { return x; }

  // Functors
  struct Overloaded
  {
    int operator()(int) const { return 1; }

    long operator()(int, int) const { return 2L; }

    template <typename T>
    T operator()(T t) const { return t; }
  };

  struct Members
  {
    int f(int) { return 1; }
    int fc(int) const { return 2; }
    int fv(int) volatile { return 3; }
    int fcv(int) const volatile { return 4; }

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
    int fn(int) noexcept { return 5; }
    int fcn(int) const noexcept { return 6; }
    int fvn(int) volatile noexcept { return 7; }
    int fcvn(int) const volatile noexcept { return 8; }
#endif
  };
}

namespace
{
  SUITE(test_is_invocable)
  {
    //*************************************************************************
    // Free function pointer invocability
    TEST(free_function_basic)
    {
      static auto temp = f0();
      (void)temp;

      // Exact match
      CHECK_TRUE((etl::is_invocable<decltype(f0)>::value));
      CHECK_TRUE((etl::is_invocable_r<int,  decltype(f0)>::value));
      CHECK_TRUE((etl::is_invocable_r<void, decltype(f0)>::value));

      // Wrong arity
      CHECK_FALSE((etl::is_invocable<decltype(f0), int>::value));

      // Return type not convertible
      CHECK_FALSE((etl::is_invocable_r<std::string, decltype(f0)>::value));
    }

    //*************************************************************************
    TEST(free_function_with_args_and_conversions)
    {
      static auto temp = f1(1.2, nullptr);
      (void)temp;

      // Exact match
      CHECK_TRUE((etl::is_invocable<decltype(f1), double, const char*>::value));
      CHECK_TRUE((etl::is_invocable_r<long,   decltype(f1), double, const char*>::value));
      CHECK_TRUE((etl::is_invocable_r<void,   decltype(f1), double, const char*>::value));
      CHECK_TRUE((etl::is_invocable_r<double, decltype(f1), double, const char*>::value)); // return convertible

      // Wrong arity
      CHECK_FALSE((etl::is_invocable<decltype(f1), double>::value));

      // Wrong return type
      CHECK_FALSE((etl::is_invocable_r<std::string, decltype(f1), double, const char*>::value));
    }

    //*************************************************************************
    TEST(void_return_conversion)
    {
      fv(1);

      using FPV = decltype(&fv);
      CHECK_TRUE((etl::is_invocable<FPV, int>::value));
      CHECK_TRUE((etl::is_invocable_r<void, FPV, int>::value));
      CHECK_FALSE((etl::is_invocable_r<int,  FPV, int>::value)); // void not convertible to int
    }

    //*************************************************************************
    TEST(noexcept_free_function_pointer)
    {
      static auto temp = fn_noexcept(1);
      (void)temp;

      using FPFN = decltype(&fn_noexcept);
      CHECK_TRUE((etl::is_invocable<FPFN, int>::value));
      CHECK_TRUE((etl::is_invocable_r<int,  FPFN, int>::value));
      CHECK_TRUE((etl::is_invocable_r<void, FPFN, int>::value));
      CHECK_FALSE((etl::is_invocable<FPFN>::value)); // wrong arity
    }

    //*************************************************************************
    TEST(member_function_pointers_object_and_pointer)
    {
      using P   = int (Members::*)(int);
      using PC  = int (Members::*)(int) const;
      using PV  = int (Members::*)(int) volatile;
      using PCV = int (Members::*)(int) const volatile;

      // Non-const member function: object and pointer
      CHECK_TRUE((etl::is_invocable<P,  Members&,  int>::value));
      CHECK_TRUE((etl::is_invocable<P,  Members*,  int>::value));
      CHECK_TRUE((etl::is_invocable_r<int,  P, Members&, int>::value));
      CHECK_TRUE((etl::is_invocable_r<void, P, Members&, int>::value));

      // Wrong arity / param type
      CHECK_FALSE((etl::is_invocable<P, Members&>::value));
      CHECK_FALSE((etl::is_invocable<P, Members&, const char*>::value));

      // const member function: requires const object
      CHECK_TRUE((etl::is_invocable<PC,  const Members&, int>::value));
      CHECK_TRUE((etl::is_invocable<PC,  const Members*, int>::value));
      CHECK_FALSE((etl::is_invocable<PC,  Members&, const char*>::value));

      // volatile member function: requires volatile object
      CHECK_TRUE((etl::is_invocable<PV,  volatile Members&, int>::value));
      CHECK_TRUE((etl::is_invocable<PV,  volatile Members*, int>::value));

      // const volatile member function
      CHECK_TRUE((etl::is_invocable<PCV, const volatile Members&, int>::value));
      CHECK_TRUE((etl::is_invocable<PCV, const volatile Members*, int>::value));
    }

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
    //*************************************************************************
    TEST(member_function_pointers_noexcept)
    {
      using PN   = int (Members::*)(int) noexcept;
      using PCN  = int (Members::*)(int) const noexcept;
      using PVN  = int (Members::*)(int) volatile noexcept;
      using PCVN = int (Members::*)(int) const volatile noexcept;

      CHECK_TRUE((etl::is_invocable<PN,   Members&, int>::value));
      CHECK_TRUE((etl::is_invocable<PCN,  const Members&, int>::value));
      CHECK_TRUE((etl::is_invocable<PVN,  volatile Members&, int>::value));
      CHECK_TRUE((etl::is_invocable<PCVN, const volatile Members&, int>::value));

      CHECK_TRUE((etl::is_invocable_r<int,  PN,  Members&, int>::value));
      CHECK_TRUE((etl::is_invocable_r<void, PN,  Members&, int>::value));

      // Negative
      CHECK_FALSE((etl::is_invocable<PN, Members&>::value));
      CHECK_FALSE((etl::is_invocable<PCN, Members&, const char*>::value));
    }
#endif

    //*************************************************************************
    TEST(functor_overloads_and_template)
    {
      using F = Overloaded;

      // Overloaded operator()(int)
      CHECK_TRUE((etl::is_invocable<F, int>::value));
      CHECK_TRUE((etl::is_invocable_r<int,  F, int>::value));
      CHECK_TRUE((etl::is_invocable_r<void, F, int>::value));
      CHECK_FALSE((etl::is_invocable<F>::value)); // wrong arity

      // Overloaded operator()(int,int)
      CHECK_TRUE((etl::is_invocable<F, int, int>::value));
      CHECK_TRUE((etl::is_invocable_r<long, F, int, int>::value));

      // Templated operator()(T)
      CHECK_TRUE((etl::is_invocable<F, double>::value));
      CHECK_TRUE((etl::is_invocable_r<double, F, double>::value));

      // Negative: wrong param type (no const char* -> int for operator()(int))
      CHECK_FALSE((etl::is_invocable_r<int, F, const char*>::value));
    }

    //*************************************************************************
    TEST(lambda_invocability)
    {
      auto lambda = [](double x) -> int { return static_cast<int>(x) + 1; };
      static auto temp = lambda(1.2);
      (void)temp;

      using L = decltype(lambda);

      CHECK_TRUE((etl::is_invocable<L, double>::value));
      CHECK_TRUE((etl::is_invocable_r<int,  L, double>::value));
      CHECK_TRUE((etl::is_invocable_r<void, L, double>::value));

      // Parameter conversion int -> double
      CHECK_TRUE((etl::is_invocable<L, int>::value));
      CHECK_TRUE((etl::is_invocable_r<int, L, int>::value));

      // Wrong arity
      CHECK_FALSE((etl::is_invocable<L>::value));
    }
  };
}