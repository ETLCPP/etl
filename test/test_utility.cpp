/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/memory.h"
#include "etl/utility.h"

#include <algorithm>
#include <map>
#include <type_traits>
#include <vector>

#include "data.h"

namespace
{
  bool nonConstCalled;
  bool constCalled;

  void TestText(std::string&)
  {
    nonConstCalled = true;
  }

  void TestText(const std::string&)
  {
    constCalled = true;
  }

  int TestGlobal(int i)
  {
    return 2 * i;
  }

  using ItemM1 = TestDataM<int>;
  using ItemM2 = TestDataM<double>;

  //*****************************************************************************
  // The test class to call.
  class TestClass
  {
  public:

    int MemberFunction(int i)
    {
      return 2 * i;
    }

    int operator()(int i)
    {
      return 2 * i;
    }
  };

  static TestClass test;
} // namespace

namespace
{
  SUITE(test_utility)
  {
    //*************************************************************************
    TEST(pair_default_construct)
    {
      etl::pair<int, double> p1;

      CHECK_EQUAL(int(), p1.first);
      CHECK_EQUAL(double(), p1.second);
    }

    //*************************************************************************
    TEST(test_pair_construct)
    {
      etl::pair<int, double> p1(1, 2.3);

      CHECK_EQUAL(1, p1.first);
      CHECK_EQUAL(2.3, p1.second);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_cpp17_deduced_pair_construct)
    {
      etl::pair p1{1, 2.3};

      CHECK((std::is_same_v<decltype(p1.first), int>));
      CHECK((std::is_same_v<decltype(p1.second), double>));

      CHECK_EQUAL(1, p1.first);
      CHECK_EQUAL(2.3, p1.second);
    }
#endif

    //*************************************************************************
    TEST(test_pair_move_parameter_construct)
    {
      etl::pair<ItemM1, ItemM2> p1(1, 2.3);

      CHECK_EQUAL(1, p1.first.value);
      CHECK_EQUAL(2.3, p1.second.value);
    }

    //*************************************************************************
    TEST(test_pair_copy_construct)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2(p1);

      CHECK_EQUAL(p1.first, p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_move_construct)
    {
      etl::pair<ItemM1, ItemM2> p1(1, 2.3);
      etl::pair<ItemM1, ItemM2> p2(std::move(p1));

      CHECK(!bool(p1.first));
      CHECK(!bool(p1.second));

      CHECK_EQUAL(1, p2.first.value);
      CHECK_EQUAL(2.3, p2.second.value);
    }

    //*************************************************************************
    TEST(test_pair_copy_construct_alternate)
    {
      etl::pair<char, float> p1(1, 2.3f);
      etl::pair<int, double> p2(p1);

      CHECK_EQUAL(p1.first, p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_make_pair)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2;
      p2 = etl::make_pair(1, 2.3);

      CHECK_EQUAL(p1.first, p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_make_pair_move)
    {
      etl::pair<ItemM1, ItemM2> p1(1, 2.3);
      etl::pair<ItemM1, ItemM2> p2(0, 0);

#include "etl/private/diagnostic_pessimizing_move_push.h"
      p2 = etl::make_pair(std::move(ItemM1(1)), std::move(ItemM2(2.3)));
#include "etl/private/diagnostic_pop.h"

      CHECK_EQUAL(p1.first, p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_swap_member)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2(2, 3.4);

      p1.swap(p2);

      CHECK_EQUAL(2, p1.first);
      CHECK_EQUAL(3.4, p1.second);

      CHECK_EQUAL(1, p2.first);
      CHECK_EQUAL(2.3, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_swap_global)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2(2, 3.4);

      swap(p1, p2);

      CHECK_EQUAL(2, p1.first);
      CHECK_EQUAL(3.4, p1.second);

      CHECK_EQUAL(1, p2.first);
      CHECK_EQUAL(2.3, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_conditional)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2(1, 2.3);
      etl::pair<int, double> p3(2, 3.4);

      CHECK(p1 == p2);
      CHECK(!(p1 == p3));
      CHECK(p1 != p3);
      CHECK(!(p1 != p2));
      CHECK(p1 <= p2);
      CHECK(p1 <= p3);
      CHECK(!(p1 < p2));
      CHECK(p1 < p3);
      CHECK(!(p3 < p1));
      CHECK(p1 >= p2);
      CHECK(!(p1 >= p3));
      CHECK(!(p1 > p2));
      CHECK(!(p1 > p3));
      CHECK(p3 > p1);
    }

    //*************************************************************************
    TEST(test_pair_conversion)
    {
#if ETL_USING_CPP11
      etl::pair<int, std::string> ep1(1, "Hello");
      std::pair<int, std::string> sp1(2, "World");

      etl::pair<int, std::string> ep2(sp1);
      std::pair<int, std::string> sp2(ep1);

      etl::pair<const int, std::string> ep3(1, "Hello");
      std::pair<const int, std::string> sp3(2, "World");

      etl::pair<int, std::string> ep4(sp3);
      std::pair<int, std::string> sp4(ep3);

      etl::pair<const int, std::string> ep5(sp1);
      std::pair<const int, std::string> sp5(ep1);

      CHECK_EQUAL(2, ep2.first);
      CHECK_EQUAL(std::string("World"), ep2.second);

      CHECK_EQUAL(1, sp2.first);
      CHECK_EQUAL(std::string("Hello"), sp2.second);

      CHECK_EQUAL(2, ep4.first);
      CHECK_EQUAL(std::string("World"), ep4.second);

      CHECK_EQUAL(1, sp4.first);
      CHECK_EQUAL(std::string("Hello"), sp4.second);

      CHECK_EQUAL(2, ep5.first);
      CHECK_EQUAL(std::string("World"), ep5.second);

      CHECK_EQUAL(1, sp5.first);
      CHECK_EQUAL(std::string("Hello"), sp5.second);
#endif
    }

    //*************************************************************************
    TEST(test_pair_move_conversion)
    {
      etl::pair<int, std::string> ep1(1, "Hello");
      std::pair<int, std::string> sp1(2, "World");

      etl::pair<int, std::string> ep2(std::move(sp1));
      std::pair<int, std::string> sp2(std::move(ep1));

      CHECK_EQUAL(2, ep2.first);
      CHECK_EQUAL(std::string("World"), ep2.second);

      CHECK_EQUAL(1, sp2.first);
      CHECK_EQUAL(std::string("Hello"), sp2.second);
    }

    //*************************************************************************
    TEST(test_exchange)
    {
      int a = 1;
      int b = 2;
      int c = etl::exchange(a, b); // c = a, a = b

      CHECK_EQUAL(2, a);
      CHECK_EQUAL(2, b);
      CHECK_EQUAL(1, c);
    }

    //*************************************************************************
    TEST(test_exchange_const)
    {
      int       a = 1;
      const int b = 2;
      int       c = etl::exchange(a, b); // c = a, a = b

      CHECK_EQUAL(2, a);
      CHECK_EQUAL(2, b);
      CHECK_EQUAL(1, c);
    }

    //*************************************************************************
    TEST(test_exchange_unique_ptr)
    {
      etl::unique_ptr<int> p1(new int(1));
      etl::unique_ptr<int> p2 = etl::exchange(p1, nullptr);

      CHECK_FALSE(p1);
      CHECK_TRUE(p2);
      CHECK_EQUAL(*p2, 1);
    }

    //*************************************************************************
    TEST(test_exchange_unique_ptr_move_in)
    {
      etl::unique_ptr<int> a(new int(10));
      etl::unique_ptr<int> b(new int(20));

      etl::unique_ptr<int> old = etl::exchange(a, etl::move(b));

      CHECK_TRUE(old);
      CHECK_EQUAL(*old, 10);
      CHECK_TRUE(a);
      CHECK_EQUAL(*a, 20);
      CHECK_FALSE(b);
    }

    //*************************************************************************
    TEST(test_as_const)
    {
      std::string text = "Hello World!";

      nonConstCalled = false;
      constCalled    = false;

      TestText(text);

      CHECK(nonConstCalled);
      CHECK(!constCalled);

      nonConstCalled = false;
      constCalled    = false;

      TestText(etl::as_const(text));

      CHECK(!nonConstCalled);
      CHECK(constCalled);
    }

    //*************************************************************************
    TEST(test_select1st)
    {
      typedef etl::pair<int, std::string> EtlPair;
      typedef std::pair<int, std::string> StdPair;

      EtlPair ep1(1, "Hello");
      StdPair sp2(2, "World");

      auto selector = etl::select1st<EtlPair>();

      CHECK_EQUAL(1, selector(ep1));
      CHECK_EQUAL(2, selector(sp2));
    }

    //*************************************************************************
    TEST(test_select1st_example)
    {
      //! [test_select1st_example]
      using Map    = std::map<int, double>;
      using Vector = std::vector<int>;

      const Map map = {{1, 0.3}, {47, 0.8}, {33, 0.1}};
      Vector    result{};

      // Extract the map keys into a vector
      std::transform(map.begin(), map.end(), std::back_inserter(result), etl::select1st<Map::value_type>());
      //! [test_select1st_example]

      CHECK_EQUAL(3, result.size());

      const Vector expected{1, 33, 47};
      CHECK_ARRAY_EQUAL(expected, result, 3);
    }

    //*************************************************************************
    TEST(test_select2nd)
    {
      typedef etl::pair<int, std::string> EtlPair;
      typedef std::pair<int, std::string> StdPair;

      EtlPair ep1(1, "Hello");
      StdPair sp2(2, "World");

      auto selector = etl::select2nd<EtlPair>();
      CHECK_EQUAL(std::string("Hello"), selector(ep1));
      CHECK_EQUAL(std::string("World"), selector(sp2));
    }

    //*************************************************************************
    TEST(test_select2nd_example)
    {
      //! [test_select2nd_example]
      using Map    = std::map<int, double>;
      using Vector = std::vector<double>;

      const Map map = {{1, 0.3}, {47, 0.8}, {33, 0.1}};
      Vector    result{};

      // Extract the map values into a vector
      std::transform(map.begin(), map.end(), std::back_inserter(result), etl::select2nd<Map::value_type>());
      //! [test_select2nd_example]

      CHECK_EQUAL(3, result.size());

      const Vector expected{0.1, 0.3, 0.8};
      sort(result.begin(), result.end()); // sort for comparison
      CHECK_ARRAY_CLOSE(expected, result, 3, 0.0001);
    }

    //*************************************************************************
    TEST(test_functor_deprecated)
    {
      constexpr etl::functor<int, int> fw1(TestGlobal);
      CHECK_EQUAL(2, fw1(1));
    }

    //*************************************************************************
    TEST(test_member_function_wrapper_deprecated)
    {
      constexpr int (*pf)(int) = &etl::member_function_wrapper<int(int)>::function<TestClass, test, &TestClass::MemberFunction>;

      CHECK_EQUAL(2, pf(1));
    }

    //*************************************************************************
    TEST(test_functor_wrapper_deprecated)
    {
      constexpr int (*pf)(int) = &etl::functor_wrapper<int(int)>::function<TestClass, test>;

      CHECK_EQUAL(2, pf(1));
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST(test_function_as_functor)
    {
      constexpr etl::function_as_functor<TestGlobal> faf;

      CHECK_EQUAL(2, faf(1));
    }
#endif

    //*************************************************************************
    TEST(test_function_ptr_as_functor)
    {
      using function_type = decltype(TestGlobal);

      constexpr function_type* fptr = TestGlobal;

      constexpr etl::function_ptr_as_functor<function_type> fpaf(fptr);

      CHECK_EQUAL(2, fpaf(1));
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST(test_functor_as_static)
    {
      using fas_t = etl::functor_as_static<test>;

      CHECK_EQUAL(2, fas_t::call(1));
    }
#endif

    //*************************************************************************
#if ETL_USING_CPP17
    TEST(test_member_function_as_static)
    {
      using mfas_t = etl::member_function_as_static<&TestClass::MemberFunction, test>;

      CHECK_EQUAL(2, mfas_t::call(1));
    }
#endif

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_member_function_as_functor)
    {
      constexpr etl::member_function_as_functor<&TestClass::MemberFunction, test> mfaf;

      CHECK_EQUAL(2, mfaf(1));
    }
#endif

    //*************************************************************************
    struct SF
    {
    };

    //*********************************
    enum class forward_call_type
    {
      LValue,
      ConstLValue,
      RValue,
      ConstRValue
    };

    //*********************************
    std::ostream& operator<<(std::ostream& os, forward_call_type type)
    {
      switch (type)
      {
        case forward_call_type::LValue:
          {
            os << "LValue";
            break;
          }

        case forward_call_type::ConstLValue:
          {
            os << "ConstLValue";
            break;
          }

        case forward_call_type::RValue:
          {
            os << "RValue";
            break;
          }

        case forward_call_type::ConstRValue:
          {
            os << "ConstRValue";
            break;
          }

        default:
          {
            os << "Unknown type";
            break;
          }
      }

      return os;
    }

    //*********************************
    forward_call_type function_f(SF&)
    {
      return forward_call_type::LValue;
    }

    //*********************************
    forward_call_type function_f(const SF&)
    {
      return forward_call_type::ConstLValue;
    }

    //*********************************
    forward_call_type function_f(SF&&)
    {
      return forward_call_type::RValue;
    }

    //*********************************
    forward_call_type function_f(const SF&&)
    {
      return forward_call_type::ConstRValue;
    }

    //*********************************
    template <typename T>
    forward_call_type template_function_f(T && t)
    {
      return function_f(etl::forward<T>(t));
    }

    //*********************************
    TEST(test_forward)
    {
      SF       s1;
      const SF s2;

      CHECK_EQUAL(forward_call_type::LValue, template_function_f(s1));
      CHECK_EQUAL(forward_call_type::RValue, template_function_f(etl::move(s1)));
      CHECK_EQUAL(forward_call_type::ConstLValue, template_function_f(s2));
      CHECK_EQUAL(forward_call_type::ConstRValue, template_function_f(etl::move(s2)));
    }

    //*************************************************************************
    struct TFL
    {
    };

    struct UFL
    {
    };

    enum class forward_like_call_type
    {
      LValue,
      ConstLValue,
      RValue,
      ConstRValue
    };

    //*********************************
    std::ostream& operator<<(std::ostream& os, forward_like_call_type type)
    {
      switch (type)
      {
        case forward_like_call_type::LValue:
          {
            os << "LValue";
            break;
          }

        case forward_like_call_type::ConstLValue:
          {
            os << "ConstLValue";
            break;
          }

        case forward_like_call_type::RValue:
          {
            os << "RValue";
            break;
          }

        case forward_like_call_type::ConstRValue:
          {
            os << "ConstRValue";
            break;
          }

        default:
          {
            os << "Unknown type";
            break;
          }
      }

      return os;
    }

    //*********************************
    forward_like_call_type function_fl(UFL&)
    {
      return forward_like_call_type::LValue;
    }

    //*********************************
    forward_like_call_type function_fl(const UFL&)
    {
      return forward_like_call_type::ConstLValue;
    }

    //*********************************
    forward_like_call_type function_fl(UFL&&)
    {
      return forward_like_call_type::RValue;
    }

    //*********************************
    forward_like_call_type function_fl(const UFL&&)
    {
      return forward_like_call_type::ConstRValue;
    }

    //*********************************
    template <typename T, typename U>
    forward_like_call_type template_function_fl(U && u)
    {
      return function_fl(etl::forward_like<T>(u));
    }

    //*********************************
    TEST(test_forward_like)
    {
      UFL        u1;
      const UFL  u2;
      UFL&       u3 = u1;
      const UFL& u4 = u2;

      CHECK_EQUAL(forward_like_call_type::LValue, template_function_fl<TFL&>(u1));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(u1));
      CHECK_EQUAL(forward_like_call_type::RValue, template_function_fl<TFL&&>(u1));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(u1));

      CHECK_EQUAL(forward_like_call_type::LValue, template_function_fl<TFL&>(etl::move(u1)));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(etl::move(u1)));
      CHECK_EQUAL(forward_like_call_type::RValue, template_function_fl<TFL&&>(etl::move(u1)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(etl::move(u1)));

      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<TFL&>(u2));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(u2));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<TFL&&>(u2));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(u2));

      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<TFL&>(etl::move(u2)));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(etl::move(u2)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<TFL&&>(etl::move(u2)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(etl::move(u2)));

      CHECK_EQUAL(forward_like_call_type::LValue, template_function_fl<TFL&>(u3));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(u3));
      CHECK_EQUAL(forward_like_call_type::RValue, template_function_fl<TFL&&>(u3));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(u3));

      CHECK_EQUAL(forward_like_call_type::LValue, template_function_fl<TFL&>(etl::move(u3)));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(etl::move(u3)));
      CHECK_EQUAL(forward_like_call_type::RValue, template_function_fl<TFL&&>(etl::move(u3)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(etl::move(u3)));

      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<TFL&>(u4));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(u4));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<TFL&&>(u4));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(u4));

      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<TFL&>(etl::move(u4)));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(etl::move(u4)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<TFL&&>(etl::move(u4)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(etl::move(u4)));
    }

#if ETL_USING_BUILTIN_UNDERLYING_TYPE
    //*********************************
    TEST(test_to_underlying)
    {
      enum enum0_t : int8_t
      {
        a0 = 'e',
        a1 = 't',
        a2 = 'l',
        a3 = '3'
      };

      enum class enum1_t : unsigned char
      {
        b0 = 2,
        b1 = 3,
        b2 = 5,
      };

      enum enum2_t : int32_t
      {
        c0 = -2,
        c1 = 100,
      };

      enum0_t e0 = enum0_t::a1;
      enum1_t e1 = enum1_t::b2;
      enum2_t e2 = enum2_t::c0;

      CHECK_TRUE((etl::is_same<int8_t, etl::underlying_type_t<enum0_t>>::value));
      CHECK_TRUE((etl::is_same<unsigned char, etl::underlying_type_t<enum1_t>>::value));
      CHECK_TRUE((etl::is_same<int32_t, etl::underlying_type_t<enum2_t>>::value));

      CHECK_EQUAL(etl::to_underlying(e0), 't');
      CHECK_EQUAL(etl::to_underlying(e1), 5);
      CHECK_EQUAL(etl::to_underlying(e2), -2);
      CHECK_EQUAL(etl::to_underlying(enum0_t::a0), 'e');
      CHECK_EQUAL(etl::to_underlying(enum0_t::a2), 'l');
      CHECK_EQUAL(etl::to_underlying(enum0_t::a3), '3');
      CHECK_EQUAL(etl::to_underlying(enum1_t::b1), 3);
      CHECK_EQUAL(etl::to_underlying(enum2_t::c1), 100);
    }
#endif

    //*********************************
    TEST(test_cmp_equal)
    {
      // Same signedness.
      CHECK_TRUE(etl::cmp_equal(1, 1));
      CHECK_FALSE(etl::cmp_equal(1, 2));
      CHECK_TRUE(etl::cmp_equal(1U, 1U));
      CHECK_FALSE(etl::cmp_equal(1U, 2U));

      // Mixed signedness where the built-in operator would give the wrong result.
      // -1 as unsigned would be a large value, so a naive (a == b) would be false anyway,
      // but the classic trap is comparing a negative signed with an equal-bit-pattern unsigned.
      CHECK_FALSE(etl::cmp_equal(-1, static_cast<unsigned int>(-1)));
      CHECK_FALSE(etl::cmp_equal(static_cast<unsigned int>(-1), -1));

      // Non-negative signed compared with unsigned.
      CHECK_TRUE(etl::cmp_equal(5, 5U));
      CHECK_TRUE(etl::cmp_equal(5U, 5));
      CHECK_FALSE(etl::cmp_equal(-5, 5U));
      CHECK_FALSE(etl::cmp_equal(5U, -5));

      // Different widths.
      CHECK_TRUE(etl::cmp_equal(int8_t(100), int64_t(100)));
      CHECK_FALSE(etl::cmp_equal(int8_t(-1), uint64_t(0xFFFFFFFFFFFFFFFFULL)));
    }

    //*********************************
    TEST(test_cmp_not_equal)
    {
      CHECK_FALSE(etl::cmp_not_equal(1, 1));
      CHECK_TRUE(etl::cmp_not_equal(1, 2));
      CHECK_TRUE(etl::cmp_not_equal(-1, static_cast<unsigned int>(-1)));
      CHECK_FALSE(etl::cmp_not_equal(5, 5U));
      CHECK_TRUE(etl::cmp_not_equal(-5, 5U));
    }

    //*********************************
    TEST(test_cmp_less)
    {
      // Same signedness.
      CHECK_TRUE(etl::cmp_less(1, 2));
      CHECK_FALSE(etl::cmp_less(2, 1));
      CHECK_FALSE(etl::cmp_less(1, 1));
      CHECK_TRUE(etl::cmp_less(1U, 2U));
      CHECK_FALSE(etl::cmp_less(2U, 1U));

      // A negative signed value is always less than any unsigned value.
      CHECK_TRUE(etl::cmp_less(-1, 0U));
      CHECK_TRUE(etl::cmp_less(-1, static_cast<unsigned int>(-1)));
      CHECK_FALSE(etl::cmp_less(static_cast<unsigned int>(-1), -1));

      // Non-negative signed with unsigned.
      CHECK_TRUE(etl::cmp_less(5, 6U));
      CHECK_FALSE(etl::cmp_less(6, 5U));
      CHECK_TRUE(etl::cmp_less(5U, 6));
      CHECK_FALSE(etl::cmp_less(6U, 5));

      // Large unsigned is not less than a small signed.
      CHECK_FALSE(etl::cmp_less(0xFFFFFFFFU, 1));
      CHECK_TRUE(etl::cmp_less(1, 0xFFFFFFFFU));
    }

    //*********************************
    TEST(test_cmp_greater)
    {
      CHECK_TRUE(etl::cmp_greater(2, 1));
      CHECK_FALSE(etl::cmp_greater(1, 2));
      CHECK_FALSE(etl::cmp_greater(1, 1));

      CHECK_FALSE(etl::cmp_greater(-1, 0U));
      CHECK_TRUE(etl::cmp_greater(static_cast<unsigned int>(-1), -1));
      CHECK_TRUE(etl::cmp_greater(0xFFFFFFFFU, 1));
      CHECK_FALSE(etl::cmp_greater(1, 0xFFFFFFFFU));
    }

    //*********************************
    TEST(test_cmp_less_equal)
    {
      CHECK_TRUE(etl::cmp_less_equal(1, 1));
      CHECK_TRUE(etl::cmp_less_equal(1, 2));
      CHECK_FALSE(etl::cmp_less_equal(2, 1));

      CHECK_TRUE(etl::cmp_less_equal(-1, static_cast<unsigned int>(-1)));
      CHECK_FALSE(etl::cmp_less_equal(static_cast<unsigned int>(-1), -1));
      CHECK_TRUE(etl::cmp_less_equal(5U, 5));
      CHECK_FALSE(etl::cmp_less_equal(6U, 5));
    }

    //*********************************
    TEST(test_cmp_greater_equal)
    {
      CHECK_TRUE(etl::cmp_greater_equal(1, 1));
      CHECK_TRUE(etl::cmp_greater_equal(2, 1));
      CHECK_FALSE(etl::cmp_greater_equal(1, 2));

      CHECK_FALSE(etl::cmp_greater_equal(-1, static_cast<unsigned int>(-1)));
      CHECK_TRUE(etl::cmp_greater_equal(static_cast<unsigned int>(-1), -1));
      CHECK_TRUE(etl::cmp_greater_equal(5, 5U));
      CHECK_FALSE(etl::cmp_greater_equal(-5, 5U));
    }

    //*********************************
    TEST(test_in_range)
    {
      // Signed target type.
      CHECK_TRUE(etl::in_range<int8_t>(0));
      CHECK_TRUE(etl::in_range<int8_t>(127));
      CHECK_TRUE(etl::in_range<int8_t>(-128));
      CHECK_FALSE(etl::in_range<int8_t>(128));
      CHECK_FALSE(etl::in_range<int8_t>(-129));
      CHECK_FALSE(etl::in_range<int8_t>(200));

      // Unsigned target type.
      CHECK_TRUE(etl::in_range<uint8_t>(0));
      CHECK_TRUE(etl::in_range<uint8_t>(255));
      CHECK_FALSE(etl::in_range<uint8_t>(256));
      CHECK_FALSE(etl::in_range<uint8_t>(-1));

      // Unsigned source that would be negative if reinterpreted as signed.
      CHECK_FALSE(etl::in_range<int8_t>(static_cast<unsigned int>(-1)));
      CHECK_TRUE(etl::in_range<uint32_t>(static_cast<unsigned int>(-1)));

      // Wider target always contains a narrower value.
      CHECK_TRUE(etl::in_range<int64_t>(int8_t(-1)));
      CHECK_TRUE(etl::in_range<int64_t>(uint32_t(0xFFFFFFFFU)));
    }

#if ETL_USING_CPP14
    //*********************************
    TEST(test_cmp_and_in_range_constexpr)
    {
      constexpr bool ce0 = etl::cmp_equal(1, 1);
      constexpr bool ce1 = etl::cmp_not_equal(-1, static_cast<unsigned int>(-1));
      constexpr bool ce2 = etl::cmp_less(-1, 0U);
      constexpr bool ce3 = etl::cmp_greater(static_cast<unsigned int>(-1), -1);
      constexpr bool ce4 = etl::cmp_less_equal(-1, static_cast<unsigned int>(-1));
      constexpr bool ce5 = etl::cmp_greater_equal(static_cast<unsigned int>(-1), -1);
      constexpr bool ce6 = etl::in_range<int8_t>(100);
      constexpr bool ce7 = etl::in_range<int8_t>(200);

      static_assert(ce0, "cmp_equal constexpr");
      static_assert(ce1, "cmp_not_equal constexpr");
      static_assert(ce2, "cmp_less constexpr");
      static_assert(ce3, "cmp_greater constexpr");
      static_assert(ce4, "cmp_less_equal constexpr");
      static_assert(ce5, "cmp_greater_equal constexpr");
      static_assert(ce6, "in_range true constexpr");
      static_assert(!ce7, "in_range false constexpr");

      CHECK_TRUE(ce0);
      CHECK_TRUE(ce6);
      CHECK_FALSE(ce7);
    }
#endif

#if ETL_HAS_PACKED
    //*********************************
    TEST(test_packed)
    {
      struct Unpacked
      {
        uint32_t a = 0x12345678;
        uint8_t  b = 0x9A;
        uint32_t c = 0x87654321;
      };

      ETL_PACKED_STRUCT(Packed)
      {
        uint32_t a = 0x12345678;
        uint8_t  b = 0x9A;
        uint32_t c = 0x87654321;
      };
      ETL_END_PACKED

      Unpacked unpacked;
      Packed   packed;

      CHECK_TRUE(sizeof(unpacked) > sizeof(packed));
      CHECK_EQUAL(9U, sizeof(packed));
    }
#endif

    //*********************************
    TEST(test_swap)
    {
      uint32_t a = 0x12345678;
      uint32_t b = 0x87654321;

      ETL_OR_STD::swap(a, b);

      CHECK_EQUAL(a, 0x87654321);
      CHECK_EQUAL(b, 0x12345678);

      uint32_t* p_a = &a;
      uint32_t* p_b = &b;

      ETL_OR_STD::swap(p_a, p_b);

      CHECK_EQUAL(a, 0x87654321);
      CHECK_EQUAL(b, 0x12345678);
      CHECK_EQUAL(p_a, &b);
      CHECK_EQUAL(p_b, &a);

      uint32_t a_a[4]{0x12345678, 0x23456789, 0x34567890, 0x09876543};
      uint32_t a_b[4]{0x12abcd78, 0x23abcd89, 0x34abcd90, 0x09abcd43};

      ETL_OR_STD::swap(a_a, a_b);

      CHECK_EQUAL(a_a[0], 0x12abcd78);
      CHECK_EQUAL(a_a[1], 0x23abcd89);
      CHECK_EQUAL(a_a[2], 0x34abcd90);
      CHECK_EQUAL(a_a[3], 0x09abcd43);

      CHECK_EQUAL(a_b[0], 0x12345678);
      CHECK_EQUAL(a_b[1], 0x23456789);
      CHECK_EQUAL(a_b[2], 0x34567890);
      CHECK_EQUAL(a_b[3], 0x09876543);
    }

#if ETL_USING_CPP17 && !defined(ETL_FORCE_CPP11_NONTYPE)
    //*********************************
    TEST(test_nontype_t_cpp17)
    {
      // Test with int
      CHECK_TRUE(42 == etl::nontype_t<42>::value);

      // Test with bool
      CHECK_TRUE(true == etl::nontype_t<true>::value);

      // Test with char
      CHECK_TRUE('A' == etl::nontype_t<'A'>::value);

      // Test with enum
      enum class E : uint8_t
      {
        A = 1,
        B = 2
      };
      CHECK_TRUE(E::A == etl::nontype_t<E::A>::value);
      CHECK_TRUE(E::B == etl::nontype_t<E::B>::value);
    }
#elif ETL_USING_CPP11
    enum class E : uint8_t
    {
      A = 1,
      B = 2
    };

    TEST(test_nontype_t_cpp11)
    {
      // Test with int
      CHECK_TRUE(42 == (etl::nontype_t<int, 42>::value));

      // Test with bool
      CHECK_TRUE(true == (etl::nontype_t<bool, true>::value));

      // Test with char
      CHECK_TRUE('A' == (etl::nontype_t<char, 'A'>::value));

      // Test with enum
      CHECK_TRUE(E::A == (etl::nontype_t<E, E::A>::value));
      CHECK_TRUE(E::B == (etl::nontype_t<E, E::B>::value));
    }
#endif

    //*********************************
    TEST(test_make_index_sequence_matches_expected)
    {
      using seq0    = etl::make_index_sequence<0U>;
      using seq1    = etl::make_index_sequence<1U>;
      using seq4    = etl::make_index_sequence<4U>;
      using expect0 = etl::index_sequence<>;
      using expect1 = etl::index_sequence<0U>;
      using expect4 = etl::index_sequence<0U, 1U, 2U, 3U>;

      CHECK_TRUE((std::is_same<seq0, expect0>::value));
      CHECK_TRUE((std::is_same<seq1, expect1>::value));
      CHECK_TRUE((std::is_same<seq4, expect4>::value));
    }

    //*********************************
    TEST(test_make_index_sequence_for_pack_matches_expected)
    {
      struct T1
      {
      };
      struct T2
      {
      };
      struct T3
      {
      };

      using seq12     = etl::make_index_sequence_for<T1, T2>;
      using seq123    = etl::make_index_sequence_for<T1, T2, T3>;
      using expect12  = etl::index_sequence<0U, 1U>;
      using expect123 = etl::index_sequence<0U, 1U, 2U>;

      CHECK_TRUE((std::is_same<seq12, expect12>::value));
      CHECK_TRUE((std::is_same<seq123, expect123>::value));
    }

    //*********************************
    TEST(test_make_index_sequence_for_type_list_matches_expected)
    {
      struct T1
      {
      };
      struct T2
      {
      };
      struct T3
      {
      };

      using list12    = etl::type_list<T1, T2>;
      using list123   = etl::type_list<T1, T2, T3>;
      using seq12     = etl::make_index_sequence_for<list12>;
      using seq123    = etl::make_index_sequence_for<list123>;
      using expect12  = etl::index_sequence<0U, 1U>;
      using expect123 = etl::index_sequence<0U, 1U, 2U>;

      CHECK_TRUE((std::is_same<seq12, expect12>::value));
      CHECK_TRUE((std::is_same<seq123, expect123>::value));
    }

    //*********************************
    TEST(test_make_index_sequence_with_offset_matches_expected)
    {
      using seq0    = etl::make_index_sequence_with_offset<5U, 0U>;
      using seq2    = etl::make_index_sequence_with_offset<3U, 2U>;
      using seq4    = etl::make_index_sequence_with_offset<7U, 4U>;
      using expect0 = etl::index_sequence<>;
      using expect2 = etl::index_sequence<3U, 4U>;
      using expect4 = etl::index_sequence<7U, 8U, 9U, 10U>;

      CHECK_TRUE((std::is_same<seq0, expect0>::value));
      CHECK_TRUE((std::is_same<seq2, expect2>::value));
      CHECK_TRUE((std::is_same<seq4, expect4>::value));
    }

    //*********************************
    TEST(test_index_sequence_push_front_matches_expected)
    {
      using seq0    = etl::index_sequence<>;
      using seq1    = etl::index_sequence<1U, 2U>;
      using result0 = etl::index_sequence_push_front_t<seq0, 5U>;
      using result1 = etl::index_sequence_push_front_t<seq1, 0U>;
      using expect0 = etl::index_sequence<5U>;
      using expect1 = etl::index_sequence<0U, 1U, 2U>;

      CHECK_TRUE((std::is_same<result0, expect0>::value));
      CHECK_TRUE((std::is_same<result1, expect1>::value));
    }

    //*********************************
    TEST(test_index_sequence_pop_front_matches_expected)
    {
      using seq0    = etl::index_sequence<>;
      using seq1    = etl::index_sequence<1U, 2U>;
      using result0 = etl::index_sequence_pop_front_t<seq0>;
      using result1 = etl::index_sequence_pop_front_t<seq1>;
      using expect0 = etl::index_sequence<>;
      using expect1 = etl::index_sequence<2U>;

      CHECK_TRUE((std::is_same<result0, expect0>::value));
      CHECK_TRUE((std::is_same<result1, expect1>::value));
    }

    //*********************************
    TEST(test_index_sequence_push_back_matches_expected)
    {
      using seq0    = etl::index_sequence<>;
      using seq1    = etl::index_sequence<1U, 2U>;
      using result0 = etl::index_sequence_push_back_t<seq0, 5U>;
      using result1 = etl::index_sequence_push_back_t<seq1, 3U>;
      using expect0 = etl::index_sequence<5U>;
      using expect1 = etl::index_sequence<1U, 2U, 3U>;

      CHECK_TRUE((std::is_same<result0, expect0>::value));
      CHECK_TRUE((std::is_same<result1, expect1>::value));
    }

    //*********************************
    TEST(test_index_sequence_pop_back_matches_expected)
    {
      using seq0    = etl::index_sequence<>;
      using seq1    = etl::index_sequence<1U, 2U>;
      using result0 = etl::index_sequence_pop_back_t<seq0>;
      using result1 = etl::index_sequence_pop_back_t<seq1>;
      using expect0 = etl::index_sequence<>;
      using expect1 = etl::index_sequence<1U>;

      CHECK_TRUE((std::is_same<result0, expect0>::value));
      CHECK_TRUE((std::is_same<result1, expect1>::value));
    }

    //*********************************
    TEST(test_index_sequence_cat_matches_expected)
    {
      using seq0    = etl::index_sequence<>;
      using seq1    = etl::index_sequence<0U, 1U>;
      using seq2    = etl::index_sequence<2U, 3U>;
      using result0 = etl::index_sequence_cat_t<seq0, seq1>;
      using result1 = etl::index_sequence_cat_t<seq1, seq2>;
      using expect0 = etl::index_sequence<0U, 1U>;
      using expect1 = etl::index_sequence<0U, 1U, 2U, 3U>;

      CHECK_TRUE((std::is_same<result0, expect0>::value));
      CHECK_TRUE((std::is_same<result1, expect1>::value));
    }

    //*********************************
    TEST(test_index_sequence_at_matches_expected)
    {
      // using seq0    = etl::index_sequence<>;          // This should fail to
      // compile as seq0 is empty
      using seq1 = etl::index_sequence<1U, 2U, 3U>;
      // using result0 = etl::index_sequence_at<seq0, 0>; // This should fail to
      // compile as seq0 is empty auto  ignore0 = result0; // Uses result0
      size_t result1a = etl::index_sequence_at<seq1, 0>::value;
      size_t result1b = etl::index_sequence_at<seq1, 1>::value;
      size_t result1c = etl::index_sequence_at<seq1, 2>::value;
      // size_t result1d = etl::index_sequence_at<seq1, 3>::value;  // This
      // should fail to compile as seq1 only has 3 elements auto   ignore1d =
      // result1d;                                // Uses result1d
      size_t expect1a = 1U;
      size_t expect1b = 2U;
      size_t expect1c = 3U;

      CHECK_EQUAL(expect1a, result1a);
      CHECK_EQUAL(expect1b, result1b);
      CHECK_EQUAL(expect1c, result1c);

#if ETL_USING_CPP17
      size_t result1e = etl::index_sequence_at_v<seq1, 0>;
      size_t result1f = etl::index_sequence_at_v<seq1, 1>;
      size_t result1g = etl::index_sequence_at_v<seq1, 2>;

      CHECK_EQUAL(expect1a, result1e);
      CHECK_EQUAL(expect1b, result1f);
      CHECK_EQUAL(expect1c, result1g);
#endif
    }

    //*************************************************************************
    TEST(test_pair_equality_uses_equality_operator)
    {
      // Basic equality
      etl::pair<int, int> p1(1, 2);
      etl::pair<int, int> p2(1, 2);
      etl::pair<int, int> p3(1, 3);
      etl::pair<int, int> p4(2, 2);

      CHECK_TRUE(p1 == p2);
      CHECK_FALSE(p1 == p3); // different second
      CHECK_FALSE(p1 == p4); // different first

      // Custom type where operator== and operator< can disagree
      // The old code used !(a<b) && !(a>b), which is NOT equivalent to a==b
      // for types that don't define a total order consistent with equality.
      struct WeirdType
      {
        int  value;
        bool equal_flag;

        bool operator==(const WeirdType& other) const
        {
          return equal_flag && other.equal_flag;
        }
        bool operator<(const WeirdType& other) const
        {
          return value < other.value;
        }
        bool operator>(const WeirdType& other) const
        {
          return value > other.value;
        }
      };

      WeirdType w1{1, false};
      WeirdType w2{1, false}; // same value, but equal_flag is false

      // With proper ==: w1 == w2 should be false (both equal_flags are false)
      // With old !(w1<w2)&&!(w1>w2): would be true (same value)
      etl::pair<int, WeirdType> pw1(0, w1);
      etl::pair<int, WeirdType> pw2(0, w2);

      CHECK_FALSE(pw1 == pw2); // This would FAIL with the old < > based comparison
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_pair_constexpr_copy_ctor)
    {
      constexpr etl::pair<int, int> p1(1, 2);
      constexpr etl::pair<int, int> p2(p1);
      static_assert(p2.first == 1, "constexpr pair copy ctor first");
      static_assert(p2.second == 2, "constexpr pair copy ctor second");
      CHECK(true);
    }

    //*************************************************************************
    TEST(test_coordinate_2d_constexpr_ctors)
    {
      constexpr etl::coordinate_2d<int> c1;
      constexpr etl::coordinate_2d<int> c2(3, 4);
      static_assert(c1.x == 0, "constexpr default ctor x");
      static_assert(c1.y == 0, "constexpr default ctor y");
      static_assert(c2.x == 3, "constexpr value ctor x");
      static_assert(c2.y == 4, "constexpr value ctor y");
      CHECK(true);
    }
#endif

    //*************************************************************************
    TEST(test_to_unsigned_from_int8_t)
    {
      auto umin   = etl::to_unsigned(etl::integral_limits<int8_t>::min);
      auto umax   = etl::to_unsigned(etl::integral_limits<int8_t>::max);
      auto uzero  = etl::to_unsigned(int8_t(0));
      auto uplus  = etl::to_unsigned(int8_t(50));
      auto uminus = etl::to_unsigned(int8_t(-50));

      auto umin_expected   = static_cast<uint8_t>(etl::integral_limits<int8_t>::min);
      auto umax_expected   = static_cast<uint8_t>(etl::integral_limits<int8_t>::max);
      auto uzero_expected  = static_cast<uint8_t>(0);
      auto uplus_expected  = static_cast<uint8_t>(50);
      auto uminus_expected = static_cast<uint8_t>(-50);

      CHECK((etl::is_same<decltype(umin), uint8_t>::value));
      CHECK((etl::is_same<decltype(umax), uint8_t>::value));
      CHECK((etl::is_same<decltype(uzero), uint8_t>::value));
      CHECK((etl::is_same<decltype(uplus), uint8_t>::value));
      CHECK((etl::is_same<decltype(uminus), uint8_t>::value));

      CHECK_EQUAL(int(umin_expected), int(umin));
      CHECK_EQUAL(int(umax_expected), int(umax));
      CHECK_EQUAL(int(uzero_expected), int(uzero));
      CHECK_EQUAL(int(uplus_expected), int(uplus));
      CHECK_EQUAL(int(uminus_expected), int(uminus));
    }

    //*************************************************************************
    TEST(test_to_unsigned_from_int16_t)
    {
      auto umin   = etl::to_unsigned(etl::integral_limits<int16_t>::min);
      auto umax   = etl::to_unsigned(etl::integral_limits<int16_t>::max);
      auto uzero  = etl::to_unsigned(int16_t(0));
      auto uplus  = etl::to_unsigned(int16_t(50));
      auto uminus = etl::to_unsigned(int16_t(-50));

      auto umin_expected   = static_cast<uint16_t>(etl::integral_limits<int16_t>::min);
      auto umax_expected   = static_cast<uint16_t>(etl::integral_limits<int16_t>::max);
      auto uzero_expected  = static_cast<uint16_t>(0);
      auto uplus_expected  = static_cast<uint16_t>(50);
      auto uminus_expected = static_cast<uint16_t>(-50);

      CHECK((etl::is_same<decltype(umin), uint16_t>::value));
      CHECK((etl::is_same<decltype(umax), uint16_t>::value));
      CHECK((etl::is_same<decltype(uzero), uint16_t>::value));
      CHECK((etl::is_same<decltype(uplus), uint16_t>::value));
      CHECK((etl::is_same<decltype(uminus), uint16_t>::value));

      CHECK_EQUAL(umin_expected, umin);
      CHECK_EQUAL(umax_expected, umax);
      CHECK_EQUAL(uzero_expected, uzero);
      CHECK_EQUAL(uplus_expected, uplus);
      CHECK_EQUAL(uminus_expected, uminus);
    }

    //*************************************************************************
    TEST(test_to_unsigned_from_int32_t)
    {
      auto umin   = etl::to_unsigned(etl::integral_limits<int32_t>::min);
      auto umax   = etl::to_unsigned(etl::integral_limits<int32_t>::max);
      auto uzero  = etl::to_unsigned(int32_t(0));
      auto uplus  = etl::to_unsigned(int32_t(50));
      auto uminus = etl::to_unsigned(int32_t(-50));

      auto umin_expected   = static_cast<uint32_t>(etl::integral_limits<int32_t>::min);
      auto umax_expected   = static_cast<uint32_t>(etl::integral_limits<int32_t>::max);
      auto uzero_expected  = static_cast<uint32_t>(0);
      auto uplus_expected  = static_cast<uint32_t>(50);
      auto uminus_expected = static_cast<uint32_t>(-50);

      CHECK((etl::is_same<decltype(umin), uint32_t>::value));
      CHECK((etl::is_same<decltype(umax), uint32_t>::value));
      CHECK((etl::is_same<decltype(uzero), uint32_t>::value));
      CHECK((etl::is_same<decltype(uplus), uint32_t>::value));
      CHECK((etl::is_same<decltype(uminus), uint32_t>::value));

      CHECK_EQUAL(umin_expected, umin);
      CHECK_EQUAL(umax_expected, umax);
      CHECK_EQUAL(uzero_expected, uzero);
      CHECK_EQUAL(uplus_expected, uplus);
      CHECK_EQUAL(uminus_expected, uminus);
    }

    //*************************************************************************
    TEST(test_to_unsigned_from_int64_t)
    {
      auto umin   = etl::to_unsigned(etl::integral_limits<int64_t>::min);
      auto umax   = etl::to_unsigned(etl::integral_limits<int64_t>::max);
      auto uzero  = etl::to_unsigned(int64_t(0));
      auto uplus  = etl::to_unsigned(int64_t(50));
      auto uminus = etl::to_unsigned(int64_t(-50));

      auto umin_expected   = static_cast<uint64_t>(etl::integral_limits<int64_t>::min);
      auto umax_expected   = static_cast<uint64_t>(etl::integral_limits<int64_t>::max);
      auto uzero_expected  = static_cast<uint64_t>(0);
      auto uplus_expected  = static_cast<uint64_t>(50);
      auto uminus_expected = static_cast<uint64_t>(-50);

      CHECK((etl::is_same<decltype(umin), uint64_t>::value));
      CHECK((etl::is_same<decltype(umax), uint64_t>::value));
      CHECK((etl::is_same<decltype(uzero), uint64_t>::value));
      CHECK((etl::is_same<decltype(uplus), uint64_t>::value));
      CHECK((etl::is_same<decltype(uminus), uint64_t>::value));

      CHECK_EQUAL(umin_expected, umin);
      CHECK_EQUAL(umax_expected, umax);
      CHECK_EQUAL(uzero_expected, uzero);
      CHECK_EQUAL(uplus_expected, uplus);
      CHECK_EQUAL(uminus_expected, uminus);
    }

    //*************************************************************************
    TEST(test_to_signed_from_uint8_t)
    {
      auto smin   = etl::to_signed(etl::integral_limits<uint8_t>::min);
      auto smax   = etl::to_signed(etl::integral_limits<uint8_t>::max);
      auto splus  = etl::to_signed(uint8_t(50));
      auto sminus = etl::to_signed(uint8_t(206));

      auto smin_expected   = static_cast<int8_t>(etl::integral_limits<uint8_t>::min);
      auto smax_expected   = static_cast<int8_t>(etl::integral_limits<uint8_t>::max);
      auto splus_expected  = static_cast<int8_t>(50);
      auto sminus_expected = static_cast<int8_t>(206);

      CHECK((etl::is_same<decltype(smin), int8_t>::value));
      CHECK((etl::is_same<decltype(smax), int8_t>::value));
      CHECK((etl::is_same<decltype(splus), int8_t>::value));
      CHECK((etl::is_same<decltype(sminus), int8_t>::value));

      CHECK_EQUAL(int(smin_expected), int(smin));
      CHECK_EQUAL(int(smax_expected), int(smax));
      CHECK_EQUAL(int(splus_expected), int(splus));
      CHECK_EQUAL(int(sminus_expected), int(sminus));
    }

    //*************************************************************************
    TEST(test_to_signed_from_uint16_t)
    {
      auto smin   = etl::to_signed(etl::integral_limits<uint16_t>::min);
      auto smax   = etl::to_signed(etl::integral_limits<uint16_t>::max);
      auto splus  = etl::to_signed(uint16_t(50));
      auto sminus = etl::to_signed(uint16_t(65486));

      auto smin_expected   = static_cast<int16_t>(etl::integral_limits<uint16_t>::min);
      auto smax_expected   = static_cast<int16_t>(etl::integral_limits<uint16_t>::max);
      auto splus_expected  = static_cast<int16_t>(50);
      auto sminus_expected = static_cast<int16_t>(65486);

      CHECK((etl::is_same<decltype(smin), int16_t>::value));
      CHECK((etl::is_same<decltype(smax), int16_t>::value));
      CHECK((etl::is_same<decltype(splus), int16_t>::value));
      CHECK((etl::is_same<decltype(sminus), int16_t>::value));

      CHECK_EQUAL(smin_expected, smin);
      CHECK_EQUAL(smax_expected, smax);
      CHECK_EQUAL(splus_expected, splus);
      CHECK_EQUAL(sminus_expected, sminus);
    }

    //*************************************************************************
    TEST(test_to_signed_from_uint32_t)
    {
      auto smin   = etl::to_signed(etl::integral_limits<uint32_t>::min);
      auto smax   = etl::to_signed(etl::integral_limits<uint32_t>::max);
      auto splus  = etl::to_signed(uint32_t(50));
      auto sminus = etl::to_signed(uint32_t(4294967246U));

      auto smin_expected   = static_cast<int32_t>(etl::integral_limits<uint32_t>::min);
      auto smax_expected   = static_cast<int32_t>(etl::integral_limits<uint32_t>::max);
      auto splus_expected  = static_cast<int32_t>(50);
      auto sminus_expected = static_cast<int32_t>(4294967246U);

      CHECK((etl::is_same<decltype(smin), int32_t>::value));
      CHECK((etl::is_same<decltype(smax), int32_t>::value));
      CHECK((etl::is_same<decltype(splus), int32_t>::value));
      CHECK((etl::is_same<decltype(sminus), int32_t>::value));

      CHECK_EQUAL(smin_expected, smin);
      CHECK_EQUAL(smax_expected, smax);
      CHECK_EQUAL(splus_expected, splus);
      CHECK_EQUAL(sminus_expected, sminus);
    }

    //*************************************************************************
    TEST(test_to_signed_from_uint64_t)
    {
      auto smin   = etl::to_signed(etl::integral_limits<uint64_t>::min);
      auto smax   = etl::to_signed(etl::integral_limits<uint64_t>::max);
      auto splus  = etl::to_signed(uint64_t(50));
      auto sminus = etl::to_signed(uint64_t(18446744073709551566ULL));

      auto smin_expected   = static_cast<int64_t>(etl::integral_limits<uint64_t>::min);
      auto smax_expected   = static_cast<int64_t>(etl::integral_limits<uint64_t>::max);
      auto splus_expected  = static_cast<int64_t>(50);
      auto sminus_expected = static_cast<int64_t>(18446744073709551566ULL);

      CHECK((etl::is_same<decltype(smin), int64_t>::value));
      CHECK((etl::is_same<decltype(smax), int64_t>::value));
      CHECK((etl::is_same<decltype(splus), int64_t>::value));
      CHECK((etl::is_same<decltype(sminus), int64_t>::value));

      CHECK_EQUAL(smin_expected, smin);
      CHECK_EQUAL(smax_expected, smax);
      CHECK_EQUAL(splus_expected, splus);
      CHECK_EQUAL(sminus_expected, sminus);
    }

    //*************************************************************************
    TEST(test_to_unsigned_constexpr)
    {
      const int8_t value = static_cast<int8_t>(-50);

      constexpr auto Size = etl::to_unsigned(value);

      CHECK_EQUAL(static_cast<uint8_t>(value), Size);

      int a[Size] = {0}; // Use Size in a context that requires a constant expression
      (void)a;           // Avoid unused variable warning
    }

    //*************************************************************************
    TEST(test_to_signed_constexpr)
    {
      const uint8_t value = static_cast<uint8_t>(50);

      constexpr auto Size = etl::to_signed(value);

      CHECK_EQUAL(static_cast<int8_t>(value), Size);

      int a[Size] = {0}; // Use Size in a context that requires a constant expression
      (void)a;           // Avoid unused variable warning
    }
  }
} // namespace
