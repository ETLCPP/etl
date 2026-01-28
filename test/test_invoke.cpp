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
  //*************************************************************************
  // Callable subjects
  int free_add(int a, int b) { return a + b; }
  int free_noexcept(int v) noexcept { return v; }
  int free_throw(int v) { throw v; return v; }

  // C-style variadic function
  int varfn(int first, ...) { return first; }

  //*********************************************
  struct Base
  {
    int data;
    Base(int d = 10) : data(d) {}

    int add(int v) { return data + v; }
    int add_const(int v) const { return data + v + 1; }

    int ref_only(int v)& { return data + v + 2; }
    int rref_only(int v)&& { return data + v + 3; }

    static int static_function(int v) { return v + 1; }
    int noexcept_member(int v) noexcept { return data + v; }
  };

  //*********************************************
  struct Derived : Base
  {
    Derived(int d = 20) : Base(d) {}
  };

  //*********************************************
  struct VolatileBase
  {
    int x;
    VolatileBase(int v = 0) : x(v) {}
    int read() const volatile { return x; }
  };

  //*********************************************
  struct Functor
  {
    int x;
    explicit Functor(int x_) : x(x_) {}
    int operator()(int i) { return i + x; }
  };

  //*********************************************
  struct ConstFunctor
  {
    int x;
    explicit ConstFunctor(int x_) : x(x_) {}
    int operator()(int i) const { return i + x; }
  };

  //*********************************************
  struct OverloadedFunctor
  {
    int operator()(int i)&  { return i + 1; }
    int operator()(int i)&& { return i + 2; }
  };

  //*********************************************
  struct MoveOnlyFunctor
  {
    MoveOnlyFunctor() = default;
    MoveOnlyFunctor(const MoveOnlyFunctor&) = delete;
    MoveOnlyFunctor(MoveOnlyFunctor&&) = default;
    int operator()(int i) { return i + 1; }
  };

  //*********************************************
  struct NoThrowFunctor
  {
    int operator()(int i) const noexcept { return i + 2; }
  };

  //*********************************************
  struct ThrowingFunctor
  {
    int operator()(int) { throw 42; }
  };

  //*********************************************
  struct MemberObj
  {
    MemberObj(int v) : i(v) 
    {
    }

    int i;
  };

  //*********************************************
  template <typename T>
  struct IntegralOnly
  {
    template <typename U, typename = typename etl::enable_if<etl::is_integral<U>::value>::type>
    int operator()(U u) { return static_cast<int>(u) + 1; }

    template <typename U, typename = typename etl::enable_if<!etl::is_integral<U>::value>::type, typename = void>
    int operator()(U) = delete;
  };

  //*********************************************
  // Non-capturing lambda (convertible to function pointer)
  static auto lambda_nc = [](int a, int b) { return a + b; };

#if ETL_USING_CPP14
  //*********************************************
  // Generic lambda (C++14+)
  static auto lambda_generic = [](auto x, auto y) { return x + y; };
#endif

  //*********************************************
  // Overload set (not directly a single callable type for traits)
  int  overload(int i)  { return i + 10; }
  long overload(long l) { return l + 20; }

  int takes_ptr(const int* p) { return *(p + 1); }

  //*********************************************
  struct Selective
  {
    template <typename T, etl::enable_if_t<etl::is_same<int, T>::value, int> = 0>
    int operator()(T) { return 1; }

    template <typename T, etl::enable_if_t<etl::is_same<char, T>::value, int> = 0>
    char operator()(T) { return 2; }

    template <typename T, etl::enable_if_t<!etl::is_integral<T>::value, int> = 0>
    int operator()(T) = delete;
  };
}

//*************************************************************************
// Unit tests for etl::is_invocable / etl::is_invocable_r
//*************************************************************************
SUITE(test_invoke)
{
  //*************************************************************************
  TEST(test_invoke_result)
  {
    int capture_value = 5;
    auto lambda_cap = [capture_value](int a) { return a + capture_value; };

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(free_add), int, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(free_noexcept), int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(free_throw), int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(varfn), int>, int>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Base::add),             Base, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Base::add_const),       Base, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Base::ref_only),        Base&, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Base::rref_only),       Base, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Base::noexcept_member), Base, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Base::static_function), int>, int>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Derived::add),             Derived, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Derived::add_const),       Derived, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Derived::ref_only),        Derived&, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Derived::rref_only),       Derived, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Derived::noexcept_member), Derived, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&Derived::static_function), int>, int>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&VolatileBase::read), VolatileBase>, int>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<Functor,      int>,  int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<ConstFunctor, int>,  int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<OverloadedFunctor&,  int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<OverloadedFunctor&&, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<MoveOnlyFunctor,     int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<NoThrowFunctor,      int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<ThrowingFunctor,     int>, int>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(&MemberObj::i), MemberObj&>, int&>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<IntegralOnly<int>, int>, int>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(lambda_nc), int, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(lambda_cap), int>, int>::value));
#if ETL_USING_CPP14
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(lambda_generic), int, int>, int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(lambda_generic), long, short>, long>::value));
#endif

    int (*ov_int)(int) = &overload;
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(ov_int), int>, int>::value));

    long (*ov_long)(long) = &overload;
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(ov_long), long>, long>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<Selective, int>,  int>::value));
    CHECK_TRUE((etl::is_same<etl::invoke_result_t<Selective, char>, char>::value));

    CHECK_TRUE((etl::is_same<etl::invoke_result_t<decltype(takes_ptr), const int* >, int>::value));
  }

  //*************************************************************************
  TEST(test_free_function)
  {
    CHECK_EQUAL(3, etl::invoke(free_add, 1, 2));
  }

  //*************************************************************************
  TEST(test_free_function_pointer_variable)
  {
    int (*fp)(int, int) = &free_add;

    CHECK_EQUAL(3, etl::invoke(fp, 1, 2));
  }

  //*************************************************************************
  TEST(test_function_pointer_const_qualification)
  {
    int (*const cfp)(int, int) = &free_add;

    CHECK_EQUAL(3, etl::invoke(cfp, 1, 2));
  }

  //*************************************************************************
  TEST(test_free_function_noexcept)
  {
    CHECK_EQUAL(1, etl::invoke(free_noexcept, 1));
  }

  //*************************************************************************
  TEST(test_free_function_throw)
  {
    CHECK_THROW(etl::invoke(free_throw, 1), int);
  }

  //*************************************************************************
  TEST(test_static_member_function)
  {
    CHECK_EQUAL(2, etl::invoke(&Base::static_function, 1));
  }

  //*************************************************************************
  TEST(test_member_function)
  {
    Base       base(10);
    const Base const_base(20);

    Base*       p_base = &base;
    const Base* p_const_base = &const_base;

    CHECK_EQUAL(11, etl::invoke(&Base::add, base, 1));
    CHECK_EQUAL(12, etl::invoke(&Base::add, p_base, 2));
    CHECK_EQUAL(14, etl::invoke(&Base::add_const, base, 3));
    CHECK_EQUAL(15, etl::invoke(&Base::add_const, p_base, 4));
    CHECK_EQUAL(26, etl::invoke(&Base::add_const, const_base, 5));
    CHECK_EQUAL(27, etl::invoke(&Base::add_const, p_const_base, 6));
  }

  //*************************************************************************
  TEST(test_member_function_noexcept)
  {
    Base base(10);

    CHECK_EQUAL(11, etl::invoke(&Base::noexcept_member, base, 1));
  }

  //*************************************************************************
  TEST(test_member_functions_ref_qualification)
  {
    Base base(10);
    Base& base_ref   = base;

    Base&& base_rref = Base(10);

    CHECK_EQUAL(13, etl::invoke(&Base::ref_only,  base_ref,  1));
    CHECK_EQUAL(15, etl::invoke(&Base::rref_only, std::move(base_rref), 2));
  }

  //************************************************************************* 
  TEST(test_inheritance_member_function)
  {
    Derived derived(10);
    Derived* p_derived = &derived;

    CHECK_EQUAL(11, etl::invoke(&Base::add, derived,   1));
    CHECK_EQUAL(12, etl::invoke(&Base::add, p_derived, 2));
  }

  //*************************************************************************
  TEST(test_member_function_volatile)
  {
    VolatileBase volatile_base(10);

    CHECK_EQUAL(10, etl::invoke(&VolatileBase::read, volatile_base));
  }

  //*************************************************************************
  TEST(test_member_object_pointer)
  {
    MemberObj obj(10);

    CHECK_EQUAL(10, etl::invoke(&MemberObj::i, obj));
    CHECK_EQUAL(20, etl::invoke(&MemberObj::i, obj) = 20);
  }

  //*************************************************************************
  TEST(test_functor_non_const)
  {
    Functor functor(10);

    CHECK_EQUAL(12, etl::invoke(functor, 2));
  }

  //*************************************************************************
  TEST(test_functor_const)
  {
    const ConstFunctor functor(10);

    CHECK_EQUAL(12, etl::invoke(functor, 2));
  }

  //*************************************************************************
  TEST(test_move_only_functor)
  {
    MoveOnlyFunctor functor;

    CHECK_EQUAL(11, etl::invoke(etl::move(functor), 10));
  }

  //*************************************************************************
  TEST(test_overloaded_functor_ref_qualifier)
  {
    OverloadedFunctor   functor;
    OverloadedFunctor&  functor_ref  = functor;
    OverloadedFunctor&& functor_rref = OverloadedFunctor();

    CHECK_EQUAL(11, etl::invoke(functor_ref, 10));
    CHECK_EQUAL(12, etl::invoke(etl::move(functor_rref), 10));
  }

  //*************************************************************************
  TEST(test_nothrow_and_throwing_functors)
  {
    NoThrowFunctor  nothrow_functor;
    ThrowingFunctor throwing_functor;

    CHECK_EQUAL(12, etl::invoke(nothrow_functor, 10));
    CHECK_THROW(etl::invoke(throwing_functor, 10), int);
  }

  //*************************************************************************
  TEST(test_lambda_non_capturing)
  {
    CHECK_EQUAL(3, etl::invoke(lambda_nc, 1, 2));
  }

  //*************************************************************************
  TEST(test_lambda_capturing)
  {
    int capture_value = 5;
    auto lambda_cap = [capture_value](int a) { return a + capture_value; };

    CHECK_EQUAL(6, etl::invoke(lambda_cap, 1));
  }

#if ETL_USING_CPP14
  //*************************************************************************
  TEST(test_lambda_generic)
  {
    CHECK_EQUAL(3, etl::invoke(lambda_generic, 1, 2));
    CHECK_EQUAL(3L, etl::invoke(lambda_generic, 1L, 2L));
    CHECK_CLOSE(3.5, etl::invoke(lambda_generic, 1.5, 2.0), 0.001);
  }
#endif

  //*************************************************************************
  TEST(test_template_functor_integral_only)
  {
    CHECK_EQUAL(2, etl::invoke(IntegralOnly<int>(), 1));
  }

  //*************************************************************************
  TEST(test_additional_user_defined_conversion)
  {
    struct Converter { operator int() const { return 11; } };

    CHECK_EQUAL(22, etl::invoke(free_add, Converter(), 11));  // Converter -> int
  }

  //*************************************************************************
  TEST(test_overload_set)
  {
    // Selecting explicit overload
    int (*ov_i)(int)   = &overload;
    long (*ov_l)(long) = &overload;

    CHECK_EQUAL(11, etl::invoke(ov_i, 1));
    CHECK_EQUAL(21L, etl::invoke(ov_l, 1L));
  }

  //*************************************************************************
  TEST(test_deleted_overload)
  {
    struct DeletedOverload
    {
      int operator()(double d) { return static_cast<int>(2 * d); }
      int operator()(int) = delete;
    };

    CHECK_CLOSE(6, etl::invoke(DeletedOverload(), 3.14), 0.001);
  }

  //*************************************************************************
  TEST(test_variadic_free_function)
  {
    CHECK_EQUAL(1, etl::invoke(varfn, 1));
    CHECK_EQUAL(2, etl::invoke(varfn, 2, 3.14, "test"));
  }

  //*************************************************************************
  TEST(test_array_decay_argument)
  {
    int arr[] = {1, 2, 3};

    CHECK_EQUAL(2, etl::invoke(takes_ptr, arr));
  }

  //*************************************************************************
  TEST(test_template_functor_multiple_enable_if)
  {
    CHECK_EQUAL(1, etl::invoke(Selective(), int(0)));
    CHECK_EQUAL(2, etl::invoke(Selective(), char(0)));
  }

  //*************************************************************************
  TEST(test_rvalue_ref_member_on_temporary)
  {
    struct RR
    {
      int f() && { return 1; }
      int g() &  { return 2; }
    };

    RR rr;

    CHECK_EQUAL(1, etl::invoke(&RR::f, etl::move(rr)));
    CHECK_EQUAL(2, etl::invoke(&RR::g, rr));
  }
}
