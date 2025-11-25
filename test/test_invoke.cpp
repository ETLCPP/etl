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

#include "etl/type_traits.h"
#include "etl/invoke.h"
#include "etl/functional.h"   // etl::reference_wrapper

#include <string>
#include <functional>
#include <type_traits>

namespace {
  //*********************************************
  // Test Subjects
  static int free_add(int a, int b) { return a + b; }
  static void free_inc(int& x) { ++x; }
  static short free_short(int a) { return static_cast<short>(a); }
  static int free_noexcept(int v) noexcept { return v * 3; }
  static int free_throw(int v) { if (v < 0) throw v; return v; }

  //*********************************************
  struct ReturnRefHolder
  {
    int v;
    int& get_ref() { return v; }
    const int& get_cref() const { return v; }
  };

  //*********************************************
  struct Base
  {
    int data;
    Base(int d = 10) : data(d) {}

    int add(int v) { return data + v; }
    int add_const(int v) const { return data + v + 1; }

    int ref_only(int v)& { return data + v + 2; }
    int rref_only(int v)&& { return data + v + 3; }

    static int static_fn(int v) { return v * 2; }

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
    int factor;
    explicit Functor(int f) : factor(f) {}
    int operator()(int x) { return x * factor; }
  };

  //*********************************************
  struct ConstFunctor
  {
    int bias;
    explicit ConstFunctor(int b) : bias(b) {}
    int operator()(int x) const { return x + bias; }
  };

  //*********************************************
  struct OverloadedFunctor
  {
    int operator()(int)& { return 1; }
    int operator()(int)&& { return 2; }
  };

  //*********************************************
  struct ForwardingFunctor
  {
    int l = 0;
    int r = 0;
    int operator()(int& v) { ++l; return v; }
    int operator()(int&& v) { ++r; return v; }
  };

  //*********************************************
  struct MoveOnlyFunctor
  {
    MoveOnlyFunctor() {}
    MoveOnlyFunctor(const MoveOnlyFunctor&) = delete;
    MoveOnlyFunctor(MoveOnlyFunctor&&) = default;
    int operator()(int x) { return x + 100; }
  };

  //*********************************************
  struct NothrowFunctor
  {
    int operator()() const noexcept { return 7; }
  };

  //*********************************************
  struct ThrowingFunctor
  {
    int operator()() { throw 42; }
  };

  //*********************************************
  template <typename T>
  etl::reference_wrapper<T> make_ref(T& obj)
  {
    return etl::reference_wrapper<T>(obj);
  }

  //*********************************************
  struct DataHolder
  {
    int value;
    double dval;
    DataHolder(int v, double d) : value(v), dval(d) {}
  };

  //*********************************************
  struct MemberObj
  {
    int i;
    const int ci;
    MemberObj(int v) : i(v), ci(v + 1) {}
    int get(int add) { return i + add; }
  };

#if ETL_USING_CPP14
  //*********************************************
  auto lambda_factory(int base)
  {
    return [base](int x) { return base + x; };
  }
#endif

  //*********************************************
  template <typename T>
  struct IntegralOnly
  {
    template <typename U,
      typename = typename etl::enable_if<etl::is_integral<U>::value>::type>
    int operator()(U u) { return static_cast<int>(u) + 10; }

    template <typename U,
      typename = typename etl::enable_if<!etl::is_integral<U>::value>::type,
      typename = void>
    int operator()(U) = delete;
  };

  struct PackSum
  {
    int operator()(int a0, int a1, int a2, int a3, int a4,
                   int a5, int a6, int a7, int a8, int a9) {
      return a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + a8 + a9;
    }
  };
}

SUITE(test_invoke)
{
  //*************************************************************************
  TEST(test_free_function_and_pointer)
  {
    CHECK_EQUAL(9, etl::invoke(free_add, 4, 5));
    int (*fp)(int,int) = &free_add;
    CHECK_EQUAL(7, etl::invoke(fp, 3, 4));
  }

  //*************************************************************************
  TEST(test_void_function)
  {
    int x = 10;
    etl::invoke(free_inc, x);
    CHECK_EQUAL(11, x);
  }

  //*************************************************************************
  TEST(test_static_member_function)
  {
    CHECK_EQUAL(18, etl::invoke(&Base::static_fn, 9));
  }

  //*************************************************************************
  TEST(test_lambdas_and_functors)
  {
    auto lam = [](int a, int b){ return a * b; };
    CHECK_EQUAL(12, etl::invoke(lam, 3, 4));

    Functor f(3);
    CHECK_EQUAL(21, etl::invoke(f, 7));

    ConstFunctor cf(5);
    CHECK_EQUAL(13, etl::invoke(cf, 8));
  }

  //*************************************************************************
  TEST(test_reference_wrapper_member_function)
  {
    Base b(15);
    auto ref = make_ref(b);
    int (Base::*pmf)(int) = &Base::add;
    CHECK_EQUAL(19, etl::invoke(pmf, ref, 4));
  }

  //*************************************************************************
  TEST(test_reference_wrapper_member_object)
  {
    MemberObj mo(10);
    auto ref = make_ref(mo);
    int MemberObj::* pmo = &MemberObj::i;
    CHECK_EQUAL(10, etl::invoke(pmo, ref));
  }

  //*************************************************************************
  TEST(test_inheritance)
  {
    Derived d(25);
    int (Base::*pmf)(int) = &Base::add;
    CHECK_EQUAL(30, etl::invoke(pmf, d, 5));
    CHECK_EQUAL(31, etl::invoke(pmf, &d, 6));
  }

  //*************************************************************************
  TEST(test_ref_qualified_member_functions)
  {
    Base b(10);
    // lvalue-qualified
    auto lpmf = &Base::ref_only;
    CHECK_EQUAL(13, etl::invoke(lpmf, b, 1));
    // rvalue-qualified
    auto rpmf = &Base::rref_only;
    CHECK_EQUAL(15, etl::invoke(rpmf, etl::move(b), 2));
  }

  //*************************************************************************
  TEST(test_member_object_pointer_variants)
  {
    DataHolder dh(9, 3.14);
    int DataHolder::* p_int    = &DataHolder::value;
    double DataHolder::* p_dbl = &DataHolder::dval;
    CHECK_EQUAL(9, etl::invoke(p_int, dh));
    CHECK_EQUAL(9, etl::invoke(p_int, &dh));
    CHECK_EQUAL(3.14, etl::invoke(p_dbl, &dh));
  }

  //*************************************************************************
  TEST(test_return_reference_modification)
  {
    ReturnRefHolder h{42};
    int& r = etl::invoke(&ReturnRefHolder::get_ref, h);
    CHECK_EQUAL(42, r);
    r = 99;
    CHECK_EQUAL(99, h.v);

    const ReturnRefHolder ch{100};
    const int& cr = etl::invoke(&ReturnRefHolder::get_cref, ch);
    CHECK_EQUAL(100, cr);
  }

  //*************************************************************************
  TEST(test_move_only_functor)
  {
    CHECK_EQUAL(105, etl::invoke(MoveOnlyFunctor(), 5));
  }

  //*************************************************************************
  TEST(test_forwarding_functor)
  {
    ForwardingFunctor ff;
    int v = 10;
    CHECK_EQUAL(10, etl::invoke(ff, v));      // lvalue path
    CHECK_EQUAL(20, etl::invoke(ff, 20));     // rvalue path
    CHECK_EQUAL(1, ff.l);
    CHECK_EQUAL(1, ff.r);
  }

  //*************************************************************************
  TEST(test_noexcept_runtime)
  {
    NothrowFunctor nf;
    CHECK_EQUAL(7, etl::invoke(nf));
    CHECK_EQUAL(12, etl::invoke(free_noexcept, 4));
  }

  //*************************************************************************
  TEST(test_throw_runtime)
  {
    NothrowFunctor nf;
    CHECK_EQUAL(7, etl::invoke(nf));
    CHECK_EQUAL(4, etl::invoke(free_throw, 4));
    CHECK_THROW(etl::invoke(free_throw, -4), int);
  }

  //*************************************************************************
  TEST(test_template_functor_integral_only)
  {
    IntegralOnly<int> io;
    CHECK_EQUAL(15, etl::invoke(io, 5));
  }

  //*************************************************************************
  TEST(test_overloaded_functor_ref_qualifier)
  {
    OverloadedFunctor of;
    CHECK_EQUAL(1, etl::invoke(of, 3));              // lvalue
    CHECK_EQUAL(2, etl::invoke(OverloadedFunctor(), 3)); // rvalue temporary
  }

  //*************************************************************************
  TEST(test_convertible_return)
  {
    CHECK_EQUAL(5, etl::invoke(free_short, 5));
  }

  //*************************************************************************
#if ETL_USING_CPP14
  TEST(test_chain_invoke_lambda_factory)
  {
    auto mid = lambda_factory(10);
    CHECK_EQUAL(14, etl::invoke(mid, 4));
  }
#endif

  //*************************************************************************
  TEST(test_large_argument_pack)
  {
    PackSum ps;
    CHECK_EQUAL(55, etl::invoke(ps, 1, 2 , 3, 4, 5, 6, 7, 8, 9, 10));
  }

  //*************************************************************************
  TEST(test_member_function_pointer_noexcept)
  {
    Base b(5);
    int (Base::*pmf)(int) = &Base::noexcept_member;
    CHECK_EQUAL(15, etl::invoke(pmf, b, 10));
  }

  //*************************************************************************
  TEST(test_reference_wrapper_inheritance)
  {
    Derived d(40);
    auto ref = make_ref(d);
    int (Base::*pmf)(int) = &Base::add;
    CHECK_EQUAL(50, etl::invoke(pmf, ref, 10));
  }

  //*************************************************************************
  TEST(test_raw_function_decay)
  {
    CHECK_EQUAL(9, etl::invoke(free_add, 4, 5));
  }

  //*************************************************************************
  TEST(test_volatile_member_function)
  {
    VolatileBase vb(123);
    int (VolatileBase::*pmv)() const volatile = &VolatileBase::read;
    CHECK_EQUAL(123, etl::invoke(pmv, vb));
  }

  //*************************************************************************
  TEST(test_const_data_member_access)
  {
    const MemberObj mo(77);
    int MemberObj::* pmo = &MemberObj::i;
    CHECK_EQUAL(77, etl::invoke(pmo, mo));
  }
}