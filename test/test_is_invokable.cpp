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

#include "etl/type_traits.h"

namespace
{
  // Free functions
  int f0() { return 42; }
  void fv(int) {}
  long f2(double, const char*) { return 7L; }

  // Add a noexcept free function
  int fn_noexcept(int x) noexcept { return x + 10; }

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
  SUITE(test_is_invokable)
  {
    //*************************************************************************
    // Free function pointer invocability
    TEST(free_function_basic)
    {
      // Exact match
      CHECK_TRUE((etl::is_invokable<decltype(&f0), int()>::value));

      // Return type can be void when the call is valid
      CHECK_TRUE((etl::is_invokable<decltype(&f0), void()>::value));

      // Wrong arity
      CHECK_FALSE((etl::is_invokable<decltype(&f0), void(int)>::value));
    }

    //*************************************************************************
    TEST(free_function_with_args_and_conversions)
    {
      // Exact match
      CHECK_TRUE((etl::is_invokable<decltype(&f2), long(double, const char*)>::value));

      // Return type convertible (long -> double)
      CHECK_TRUE((etl::is_invokable<decltype(&f2), double(double, const char*)>::value));
      
      // Void return is OK when call is valid
      CHECK_TRUE((etl::is_invokable<decltype(&f2), void(double, const char*)>::value));
      
      // Bad parameter types
      CHECK_FALSE((etl::is_invokable<decltype(&f2), long(int, int)>::value));
    }

    //*************************************************************************
    TEST(free_function_bad_return_conversion)
    {
      // int -> char* is not convertible
      CHECK_FALSE((etl::is_invokable<decltype(&f0), char*()>::value));
    }

    //*************************************************************************
    TEST(member_function_pointers)
    {
      using P   = int (Members::*)(int);
      using PC  = int (Members::*)(int) const;
      using PV  = int (Members::*)(int) volatile;
      using PCV = int (Members::*)(int) const volatile;

      CHECK_TRUE((etl::is_invokable<P,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<P,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<P, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<P, int(const char*)>::value)); // Wrong parameter type

      CHECK_TRUE((etl::is_invokable<PC,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PC,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<PC, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<PC, int(const char*)>::value)); // Wrong parameter type

      CHECK_TRUE((etl::is_invokable<PV,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PV,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<PV, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<PV, int(const char*)>::value)); // Wrong parameter type

      CHECK_TRUE((etl::is_invokable<PCV,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PCV,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<PCV, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<PCV, int(const char*)>::value)); // Wrong parameter type

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      using PN   = int (Members::*)(int) noexcept;
      using PCN  = int (Members::*)(int) const noexcept;
      using PVN  = int (Members::*)(int) volatile noexcept;
      using PCVN = int (Members::*)(int) const volatile noexcept;

      CHECK_TRUE((etl::is_invokable<PN,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PN,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<PN, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<PN, int(const char*)>::value)); // Wrong parameter type

      CHECK_TRUE((etl::is_invokable<PCN,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PCN,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<PCN, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<PCN, int(const char*)>::value)); // Wrong parameter type

      CHECK_TRUE((etl::is_invokable<PVN,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PVN,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<PVN, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<PVN, int(const char*)>::value)); // Wrong parameter type

      CHECK_TRUE((etl::is_invokable<PCVN,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PCVN,  void(int)>::value));
      CHECK_FALSE((etl::is_invokable<PCVN, int()>::value));            // Wrong arity
      CHECK_FALSE((etl::is_invokable<PCVN, int(const char*)>::value)); // Wrong parameter type
#endif
    }

    //*************************************************************************
    TEST(functor_overload_and_template)
    {
      using F = Overloaded;

      // Overloaded operator()(int)
      CHECK_TRUE((etl::is_invokable<F, int(int)>::value));
      CHECK_TRUE((etl::is_invokable<F, void(int)>::value));
      CHECK_FALSE((etl::is_invokable<F, int()>::value));

      // Overloaded operator()(int,int)
      CHECK_TRUE((etl::is_invokable<F, long(int, int)>::value));
      CHECK_TRUE((etl::is_invokable<F, void(int, int)>::value));
      CHECK_TRUE((etl::is_invokable<F, long(int)>::value));

      // Templated operator()(T)
      CHECK_TRUE((etl::is_invokable<F, double(double)>::value));
      CHECK_TRUE((etl::is_invokable<F, void(double)>::value));
      CHECK_FALSE((etl::is_invokable<F, int(const char*)>::value)); // no implicit conv from const char* to int
    }

    //*************************************************************************
    TEST(noexcept_functions)
    {
      // Free noexcept function pointer
      using FPFN = decltype(&fn_noexcept);
      CHECK_TRUE((etl::is_invokable<FPFN, int(int)>::value));
      CHECK_TRUE((etl::is_invokable<FPFN, void(int)>::value));
      CHECK_FALSE((etl::is_invokable<FPFN, int()>::value));

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      using PN   = int (Members::*)(int) noexcept;
      using PCN  = int (Members::*)(int) const noexcept;
      using PVN  = int (Members::*)(int) volatile noexcept;
      using PCVN = int (Members::*)(int) const volatile noexcept;

      CHECK_TRUE((etl::is_invokable<PN,   int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PCN,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PVN,  int(int)>::value));
      CHECK_TRUE((etl::is_invokable<PCVN, int(int)>::value));

      CHECK_TRUE((etl::is_invokable<PN,   void(int)>::value));
      CHECK_TRUE((etl::is_invokable<PCN,  void(int)>::value));
      CHECK_TRUE((etl::is_invokable<PVN,  void(int)>::value));
      CHECK_TRUE((etl::is_invokable<PCVN, void(int)>::value));

      // Negative: Wrong arity or type
      CHECK_FALSE((etl::is_invokable<PN, int()>::value));
      CHECK_FALSE((etl::is_invokable<PCN, int(const char*)>::value));
#endif
    }

    //*************************************************************************
    TEST(lambda_and_function_pointer)
    {
      auto lam = [](double x) -> int { return static_cast<int>(x) + 1; };
      using L = decltype(lam);

      // Lambda
      CHECK_TRUE((etl::is_invokable<L, int(double)>::value));
      CHECK_TRUE((etl::is_invokable<L, void(double)>::value));
      CHECK_TRUE((etl::is_invokable<L, int(int)>::value));

      // Function pointer to void(int)
      using FP = void(*)(int);
      CHECK_TRUE((etl::is_invokable<FP, void(int)>::value));
      CHECK_FALSE((etl::is_invokable<FP, void()>::value));
      
      // Return conversion from void -> int is not allowed
      CHECK_FALSE((etl::is_invokable<FP, int(int)>::value));

      // Exact free function pointer
      CHECK_TRUE((etl::is_invokable<decltype(&fv), void(int)>::value));
    }

    //*************************************************************************
    TEST(is_invokable_r_free_and_functor)
    {
      using F = Overloaded;

      // Free functions
      CHECK_TRUE((etl::is_invokable_r<int,    decltype(&f0)>::value));      // exact
      CHECK_TRUE((etl::is_invokable_r<void,   decltype(&f0)>::value));      // void allowed
      CHECK_FALSE((etl::is_invokable_r<void,  decltype(&f0), int>::value)); // Wrong arity

      CHECK_TRUE((etl::is_invokable_r<long,   decltype(&f2), double, const char*>::value)); // exact
      CHECK_TRUE((etl::is_invokable_r<double, decltype(&f2), double, const char*>::value)); // return convertible
      CHECK_TRUE((etl::is_invokable_r<void,   decltype(&f2), double, const char*>::value)); // void
      CHECK_FALSE((etl::is_invokable_r<long,  decltype(&f2), int, int>::value));            // Wrong params

      // Bad return conversion
      CHECK_FALSE((etl::is_invokable_r<char*, decltype(&f0)>::value));

      // Functor/lambda-like
      CHECK_TRUE((etl::is_invokable_r<int,  F, int>::value));
      CHECK_TRUE((etl::is_invokable_r<void, F, int>::value));
      CHECK_TRUE((etl::is_invokable_r<long, F, int>::value));          // int -> long
      CHECK_TRUE((etl::is_invokable_r<long, F, int, int>::value));
      CHECK_FALSE((etl::is_invokable_r<void, F>::value));              // Wrong arity
      CHECK_FALSE((etl::is_invokable_r<int,  F, const char*>::value)); // Wrong parameter type
    }

    //*************************************************************************
    TEST(is_invokable_r_member_and_lambda)
    {
      // Member function pointers
      using P   = int (Members::*)(int);
      using PC  = int (Members::*)(int) const;
      using PV  = int (Members::*)(int) volatile;
      using PCV = int (Members::*)(int) const volatile;

      CHECK_TRUE((etl::is_invokable_r<int,  P,   int>::value));
      CHECK_TRUE((etl::is_invokable_r<int,  PC,  int>::value));
      CHECK_TRUE((etl::is_invokable_r<int,  PV,  int>::value));
      CHECK_TRUE((etl::is_invokable_r<int,  PCV, int>::value));

      CHECK_FALSE((etl::is_invokable_r<char*,  P,   int>::value));
      CHECK_FALSE((etl::is_invokable_r<char*,  PC,  int>::value));
      CHECK_FALSE((etl::is_invokable_r<char*,  PV,  int>::value));
      CHECK_FALSE((etl::is_invokable_r<char*,  PCV, int>::value));

#if ETL_HAS_NOEXCEPT_FUNCTION_TYPE
      using PN   = int (Members::*)(int) noexcept;
      using PCN  = int (Members::*)(int) const noexcept;
      using PVN  = int (Members::*)(int) volatile noexcept;
      using PCVN = int (Members::*)(int) const volatile noexcept;

      CHECK_TRUE((etl::is_invokable_r<int,  PN,   int>::value));
      CHECK_TRUE((etl::is_invokable_r<int,  PCN,  int>::value));
      CHECK_TRUE((etl::is_invokable_r<int,  PVN,  int>::value));
      CHECK_TRUE((etl::is_invokable_r<int,  PCVN, int>::value));

      CHECK_FALSE((etl::is_invokable_r<char*,  PN,   int>::value));
      CHECK_FALSE((etl::is_invokable_r<char*,  PCN,  int>::value));
      CHECK_FALSE((etl::is_invokable_r<char*,  PVN,  int>::value));
      CHECK_FALSE((etl::is_invokable_r<char*,  PCVN, int>::value));
#endif

      // Lambda
      auto lam = [](double x) -> int { return static_cast<int>(x) + 1; };
      using L = decltype(lam);

      CHECK_TRUE((etl::is_invokable_r<int,  L, double>::value));
      CHECK_TRUE((etl::is_invokable_r<void, L, double>::value));
      CHECK_TRUE((etl::is_invokable_r<int,  L, int>::value)); // int -> double parameter conv
      CHECK_FALSE((etl::is_invokable_r<bad_ref, L, double>::value)); // bad return conv

      // Function pointer returning void
      using FP = void(*)(int);

      CHECK_TRUE((etl::is_invokable_r<void, FP, int>::value));
      CHECK_FALSE((etl::is_invokable_r<int,  FP, int>::value)); // void not convertible to int
    }
  };
}