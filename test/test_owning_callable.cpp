/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#include "etl/owning_callable.h"

#include <functional>

#if ETL_USING_CPP11
namespace
{
  void fooRef(int32_t& result)
  {
    result = 2;
  }

  int32_t foo0()
  {
    return 2;
  }

  int32_t foo1(float f)
  {
    return f;
  }

  int32_t foo2(float, int32_t g)
  {
    return g;
  }

  int32_t foo3(float, int32_t, int32_t h)
  {
    return h;
  }

  int32_t foo4(float, int32_t, int32_t, int32_t i)
  {
    return i;
  }

  int32_t foo5(float, int32_t, int32_t, int32_t, int32_t j)
  {
    return j;
  }

  struct Foo
  {
    int32_t foo0()
    {
      return 3;
    }

    int32_t foo1(float f)
    {
      return f;
    }

    int32_t foo2(float, int32_t g)
    {
      return g;
    }

    int32_t foo3(float, int32_t, int32_t h)
    {
      return h;
    }

    int32_t foo4(float, int32_t, int32_t, int32_t i)
    {
      return i;
    }

    int32_t foo5(float, int32_t, int32_t, int32_t, int32_t j)
    {
      return j;
    }

    static int32_t bar0()
    {
      return 5;
    }

    static int32_t bar1(float f)
    {
      return f;
    }

    static int32_t bar2(float, int32_t g)
    {
      return g;
    }

    static int32_t bar3(float, int32_t, int32_t h)
    {
      return h;
    }

    static int32_t bar4(float, int32_t, int32_t, int32_t i)
    {
      return i;
    }

    static int32_t bar5(float, int32_t, int32_t, int32_t, int32_t j)
    {
      return j;
    }
  };

  struct Bar0
  {
    int32_t operator()() const
    {
      return 7;
    }
  };

  struct Bar1
  {
    int32_t operator()(float f) const
    {
      return f;
    }
  };

  struct Bar2
  {
    int32_t operator()(float, int32_t g) const
    {
      return g;
    }
  };

  struct Bar3
  {
    int32_t operator()(float, int32_t, int32_t h) const
    {
      return h;
    }
  };

  struct Bar4
  {
    int32_t operator()(float, int32_t, int32_t, int32_t i) const
    {
      return i;
    }
  };

  struct Bar5
  {
    int32_t operator()(float, int32_t, int32_t, int32_t, int32_t j) const
    {
      return j;
    }
  };

  struct BarV
  {
    int32_t _m;

    int32_t operator()()
    {
      _m++;
      return _m + 7;
    }

    int32_t operator()(int32_t f) const
    {
      return _m + f;
    }

    int32_t operator()(int32_t, int32_t g) const
    {
      return _m + g;
    }

    int32_t operator()(int32_t, int32_t, int32_t h) const
    {
      return _m + h;
    }

    int32_t operator()(int32_t, int32_t, int32_t, int32_t i) const
    {
      return _m + i;
    }

    int32_t operator()(int32_t, int32_t, int32_t, int32_t, int32_t j) const
    {
      return _m + j;
    }
  };

  struct IntClass
  {
    IntClass()
      : i(10)
    {
    }

    int32_t i;
  };

  struct copy_count_t
  {
    int copy_cnt{0};

    explicit copy_count_t() = default;
    copy_count_t(copy_count_t const& rhs)
      : copy_cnt{rhs.copy_cnt + 1}
    {
    }

    copy_count_t& operator=(copy_count_t const& rhs)
    {
      copy_cnt = rhs.copy_cnt + 1;
      return *this;
    }
  };
}  // namespace

namespace
{
  SUITE(test_owning_callable)
  {
    TEST(DefaultConstructed)
    {
      etl::owning_callable<IntClass()> f;
      CHECK_THROW((f()), etl::bad_owning_callable_call);

      etl::owning_callable<IntClass(IntClass)> f1;
      IntClass                                  a1;
      CHECK_THROW((f1(a1)), etl::bad_owning_callable_call);

      etl::owning_callable<IntClass(IntClass, IntClass)> f2;
      IntClass                                            a2;
      CHECK_THROW((f2(a1, a2)), etl::bad_owning_callable_call);

      etl::owning_callable<IntClass(IntClass, IntClass)> f2_copy;

      f2_copy = f2;
      CHECK_THROW((f2_copy(a1, a2)), etl::bad_owning_callable_call);

      etl::owning_callable<IntClass(IntClass, IntClass, IntClass)> f3;
      IntClass                                                      a3;
      CHECK_THROW((f3(a1, a2, a3)), etl::bad_owning_callable_call);

      etl::owning_callable<IntClass(IntClass, IntClass, IntClass, IntClass)> f4;
      IntClass                                                                a4;
      CHECK_THROW((f4(a1, a2, a3, a4)), etl::bad_owning_callable_call);

      etl::owning_callable<IntClass(IntClass, IntClass, IntClass, IntClass, IntClass)> f5;
      IntClass                                                                          a5;
      CHECK_THROW((f5(a1, a2, a3, a4, a5)), etl::bad_owning_callable_call);
    }

    TEST(HasValue)
    {
      etl::owning_callable<int32_t()>                                          f0;
      etl::owning_callable<int32_t(float)>                                     f1;
      etl::owning_callable<int32_t(float, int32_t)>                            f2;
      etl::owning_callable<int32_t(float, int32_t, int32_t)>                   f3;
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t)>          f4;
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t, int32_t)> f5;

      CHECK_FALSE(f0);
      CHECK_FALSE(f1);
      CHECK_FALSE(f2);
      CHECK_FALSE(f3);
      CHECK_FALSE(f4);
      CHECK_FALSE(f5);

      f0 = etl::owning_callable<int32_t()>(&Foo::bar0);
      f1 = etl::owning_callable<int32_t(float)>(&Foo::bar1);
      f2 = etl::owning_callable<int32_t(float, int32_t)>(&Foo::bar2);
      f3 = etl::owning_callable<int32_t(float, int32_t, int32_t)>(&Foo::bar3);
      f4 = etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t)>(&Foo::bar4);
      f5 = etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t, int32_t)>(&Foo::bar5);

      CHECK(f0);
      CHECK(f1);
      CHECK(f2);
      CHECK(f3);
      CHECK(f4);
      CHECK(f5);

      f0 = nullptr;
      f1 = nullptr;
      f2 = nullptr;
      f3 = nullptr;
      f4 = nullptr;
      f5 = nullptr;

      CHECK_FALSE(f0);
      CHECK_FALSE(f1);
      CHECK_FALSE(f2);
      CHECK_FALSE(f3);
      CHECK_FALSE(f4);
      CHECK_FALSE(f5);

      f0 = etl::owning_callable<int32_t()>(&Foo::bar0);
      f1 = etl::owning_callable<int32_t(float)>(&Foo::bar1);
      f2 = etl::owning_callable<int32_t(float, int32_t)>(&Foo::bar2);
      f3 = etl::owning_callable<int32_t(float, int32_t, int32_t)>(&Foo::bar3);
      f4 = etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t)>(&Foo::bar4);
      f5 = etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t, int32_t)>(&Foo::bar5);

      CHECK(f0);
      CHECK(f1);
      CHECK(f2);
      CHECK(f3);
      CHECK(f4);
      CHECK(f5);

      CHECK_EQUAL(5, f0());
      CHECK_EQUAL(7, f1(7.58));
      CHECK_EQUAL(0, f2(6.78, 0));
      CHECK_EQUAL(7, f3(5.32, 31, 7));
      CHECK_EQUAL(2, f4(0.21, 12, 61, 2));
      CHECK_EQUAL(9, f5(3.14, 4, 3, 1, 9));
    }

    TEST(TestFreeFunctionZeroParams)
    {
      etl::owning_callable<int32_t()> f = etl::owning_callable<int32_t()>(&foo0);
      CHECK_EQUAL(2, f());
    }

    TEST(TestMemberFunctionZeroParams)
    {
      Foo                              foo;
      etl::owning_callable<int32_t()> f = std::bind(&Foo::foo0, &foo);
      CHECK_EQUAL(3, f());
    }

    TEST(TestStaticMemberFunctionZeroParams)
    {
      etl::owning_callable<int32_t()> f = etl::owning_callable<int32_t()>(&Foo::bar0);
      CHECK_EQUAL(5, f());
    }

    TEST(TestFunctorZeroParams)
    {
      Bar0                             bar;
      etl::owning_callable<int32_t()> f(bar);
      CHECK_EQUAL(7, f());
    }

    TEST(TestLambdaParameter)
    {
      auto callme = [](etl::owning_callable<int32_t()> f)
      { return f(); };
      auto f = []()
      { return 112358; };
      CHECK_EQUAL(112358, callme(f));
    }

    TEST(TestFreeFunctionOneParam)
    {
      etl::owning_callable<int32_t(float)> f = &foo1;
      CHECK_EQUAL(2, f(2.14));
    }

    TEST(TestFreeFunctionOneParamVoid)
    {
      etl::owning_callable<void(int32_t&)> f = &fooRef;
      int32_t                               result = 0;
      f(result);
      CHECK_EQUAL(2, result);
    }

    TEST(TestMemberFunctionOneParam)
    {
      Foo                                   foo;
      etl::owning_callable<int32_t(float)> f =
        std::bind(&Foo::foo1, &foo, std::placeholders::_1);
      CHECK_EQUAL(3, f(3.14));
    }

    TEST(TestStaticMemberFunctionOneParam)
    {
      etl::owning_callable<int32_t(float)> f = &Foo::bar1;
      CHECK_EQUAL(5, f(5.75));
    }

    TEST(TestFunctorOneParam)
    {
      Bar1                                  bar;
      etl::owning_callable<int32_t(float)> f = bar;
      CHECK_EQUAL(7, f(7.65));
    }

    TEST(TestFreeFunctionTwoParams)
    {
      etl::owning_callable<int32_t(float, int32_t)> f = &foo2;
      CHECK_EQUAL(2, f(2.14, 2));
    }

    TEST(TestMemberFunctionTwoParams)
    {
      Foo                                            foo;
      etl::owning_callable<int32_t(float, int32_t)> f =
        std::bind(&Foo::foo2, &foo, std::placeholders::_1, std::placeholders::_2);
      CHECK_EQUAL(3, f(3.14, 3));
    }

    TEST(TestStaticMemberFunctionTwoParams)
    {
      etl::owning_callable<int32_t(float, int32_t)> f = &Foo::bar2;
      CHECK_EQUAL(5, f(5.75, 5));
    }

    TEST(TestFunctorTwoParams)
    {
      Bar2                                           bar;
      etl::owning_callable<int32_t(float, int32_t)> f = bar;
      CHECK_EQUAL(7, f(7.65, 7));
    }

    TEST(TestFreeFunctionThreeParams)
    {
      etl::owning_callable<int32_t(float, int32_t, int32_t)> f = &foo3;
      CHECK_EQUAL(4, f(2.14, 2, 4));
    }

    TEST(TestMemberFunctionThreeParams)
    {
      Foo                                                     foo, *pfoo{&foo};
      etl::owning_callable<int32_t(float, int32_t, int32_t)> f =
        [pfoo](float p0, int32_t p1, int32_t p2)
      { return pfoo->foo3(p0, p1, p2); };
      CHECK_EQUAL(120, f(3.14, 3, 120));
    }

    TEST(TestStaticMemberFunctionThreeParams)
    {
      etl::owning_callable<int32_t(float, int32_t, int32_t)> f = &Foo::bar3;
      CHECK_EQUAL(4, f(5.75, 5, 4));
    }

    TEST(TestFunctorThreeParams)
    {
      Bar3                                                    bar;
      etl::owning_callable<int32_t(float, int32_t, int32_t)> f = bar;
      CHECK_EQUAL(4, f(7.65, 7, 4));
    }

    TEST(TestFreeFunctionFourParams)
    {
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t)> f = &foo4;
      CHECK_EQUAL(5, f(2.14, 2, 4, 5));
    }

    TEST(TestStaticMemberFunctionFourParams)
    {
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t)> f = &Foo::bar4;
      CHECK_EQUAL(5, f(5.75, 5, 4, 5));
    }

    TEST(TestFunctorFourParams)
    {
      Bar4                                                             bar;
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t)> f = bar;
      CHECK_EQUAL(5, f(7.65, 7, 4, 5));
    }

    TEST(TestFreeFunctionFiveParams)
    {
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t, int32_t)> f = &foo5;
      CHECK_EQUAL(7, f(2.14, 2, 4, 5, 7));
    }

    TEST(TestStaticMemberFunctionFiveParams)
    {
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t, int32_t)> f = &Foo::bar5;
      CHECK_EQUAL(8, f(5.75, 5, 4, 5, 8));
    }

    TEST(TestFunctorFiveParams)
    {
      Bar5                                                                      bar;
      etl::owning_callable<int32_t(float, int32_t, int32_t, int32_t, int32_t)> f =
        bar;
      CHECK_EQUAL(8, f(7.65, 7, 4, 5, 8));
    }

    TEST(CopyConstructorZeroParams)
    {
      {
        etl::owning_callable<int32_t()> f = &foo0;
        etl::owning_callable<int32_t()> g(f);
        CHECK_EQUAL(2, f());
        CHECK_EQUAL(2, g());
      }
      {
        Foo                              foo, *pfoo{&foo};
        etl::owning_callable<int32_t()> f = [pfoo]
        { return pfoo->foo0(); };
        etl::owning_callable<int32_t()> g(f);
        CHECK_EQUAL(3, f());
        CHECK_EQUAL(3, g());
      }
      {
        etl::owning_callable<int32_t()> f = &Foo::bar0;
        etl::owning_callable<int32_t()> g = f;
        CHECK_EQUAL(5, f());
        CHECK_EQUAL(5, g());
      }
      {
        Bar0                             bar;
        etl::owning_callable<int32_t()> f = bar;
        etl::owning_callable<int32_t()> g = f;
        CHECK_EQUAL(7, f());
        CHECK_EQUAL(7, g());
      }
    }

    TEST(AssignmentOperatorZeroParams)
    {
      {
        etl::owning_callable<int32_t()> f = &foo0, g;
        g = f;
        CHECK_EQUAL(2, f());
        CHECK_EQUAL(2, g());
      }
      {
        Foo                              foo, *pfoo{&foo};
        etl::owning_callable<int32_t()> f = [pfoo]
        { return pfoo->foo0(); },
                                         g;
        g = f;
        CHECK_EQUAL(3, f());
        CHECK_EQUAL(3, g());
      }
      {
        etl::owning_callable<int32_t()> f = &Foo::bar0, g;
        g = f;
        CHECK_EQUAL(5, f());
        CHECK_EQUAL(5, g());
      }
      {
        Bar0                             bar;
        etl::owning_callable<int32_t()> f = bar, g;
        g = f;
        CHECK_EQUAL(7, f());
        CHECK_EQUAL(7, g());
      }
    }

    class Const
    {
      int32_t _i;

    public:
      Const(int32_t i)
        : _i(i)
      {
      }

      int32_t get0() const
      {
        return _i;
      }

      int32_t get0_b() const
      {
        return _i * 2;
      }

      int32_t get1(Const&) const
      {
        return _i;
      }

      int32_t get1NotConst(Const const&) const
      {
        return _i;
      }

      int32_t get2(Const const&, Const const&) const
      {
        return _i;
      }

      int32_t get3(Const const&, Const const&, Const const&) const
      {
        return _i;
      }

      int32_t get4(Const const&, Const const&, Const const&, Const const&) const
      {
        return _i;
      }

      int32_t get5(Const const&, Const const&, Const const&, Const const&, Const const&) const
      {
        return _i;
      }
    };

    TEST(ConstMemberFunction)
    {
      Const                            ncon(17), *pncon{&ncon};
      etl::owning_callable<int32_t()> f = [pncon]
      { return pncon->get0(); };
      CHECK_EQUAL(17, f());

      Const const &                    con = ncon, *pcon{&con};
      etl::owning_callable<int32_t()> f2 = [pcon]
      { return pcon->get0(); };
      CHECK_EQUAL(17, f2());

      etl::owning_callable<int32_t(Const&)> f3 = [pcon](Const& p0)
      { return pcon->get1(p0); };
      CHECK_EQUAL(17, f3(ncon));

      etl::owning_callable<int32_t(Const const&)> f3NotConst =
        [pncon](Const const& p0)
      { return pncon->get1NotConst(p0); };
      CHECK_EQUAL(17, f3NotConst(ncon));

      etl::owning_callable<int32_t(Const const&, Const const&)> f4 =
        [pcon](Const const& p0, Const const& p1)
      { return pcon->get2(p0, p1); };
      CHECK_EQUAL(17, f4(ncon, ncon));
      CHECK_EQUAL(17, f4(con, con));

      etl::owning_callable<int32_t(Const const&, Const const&, Const const&)> f5 =
        [pcon](Const const& p0, Const const& p1, Const const& p2)
      { return pcon->get3(p0, p1, p2); };
      CHECK_EQUAL(17, f5(ncon, ncon, ncon));
      CHECK_EQUAL(17, f5(con, con, con));

      etl::owning_callable<int32_t(Const const&, Const const&, Const const&, Const const&)> f6 = etl::owning_callable<int32_t(Const const&, Const const&, Const const&, Const const&)>([pcon](Const const& p0, Const const& p1, Const const& p2, Const const& p3)
                                                                                                                                                                                         { return pcon->get4(p0, p1, p2, p3); });
      CHECK_EQUAL(17, f6(ncon, ncon, ncon, ncon));
      CHECK_EQUAL(17, f6(con, con, con, con));

      etl::owning_callable<int32_t(Const const&, Const const&, Const const&, Const const&, Const const&)>
        f7 = [pcon](Const const& p0, Const const& p1, Const const& p2, Const const& p3, Const const& p4)
      { return pcon->get5(p0, p1, p2, p3, p4); };
      CHECK_EQUAL(17, f7(ncon, ncon, ncon, ncon, ncon));
      CHECK_EQUAL(17, f7(con, con, con, con, con));
    }
  }

  TEST(NoRedundantCopy)
  {
    etl::owning_callable<int(copy_count_t)> f_copy =
      [](copy_count_t obj)
    { return obj.copy_cnt; };
    etl::owning_callable<int(copy_count_t const&)> f_ref =
      [](copy_count_t const& obj)
    { return obj.copy_cnt; };
    copy_count_t obj;

    CHECK_TRUE(f_copy(obj) < 3);
    CHECK_EQUAL(f_ref(obj), 0);
  }
}  // namespace

#endif
