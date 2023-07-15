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

#include "etl/utility.h"

#include <map>
#include <vector>
#include <algorithm>

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
}

namespace
{
  SUITE(test_utility)
  {
    //*************************************************************************
    TEST(pair_default_construct)
    {
      etl::pair<int, double> p1;

      CHECK_EQUAL(int(),    p1.first);
      CHECK_EQUAL(double(), p1.second);
    }

    //*************************************************************************
    TEST(test_pair_construct)
    {
      etl::pair<int, double> p1(1, 2.3);

      CHECK_EQUAL(1,   p1.first);
      CHECK_EQUAL(2.3, p1.second);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST(test_cpp17_deduced_pair_construct)
    {
      etl::pair p1{ 1, 2.3 };

      CHECK((std::is_same_v<decltype(p1.first), int>));
      CHECK((std::is_same_v<decltype(p1.second), double>));

      CHECK_EQUAL(1,   p1.first);
      CHECK_EQUAL(2.3, p1.second);
    }
#endif

    //*************************************************************************
    TEST(test_pair_move_parameter_construct)
    {
      etl::pair<ItemM1, ItemM2> p1(1, 2.3);

      CHECK_EQUAL(1,   p1.first.value);
      CHECK_EQUAL(2.3, p1.second.value);
    }

    //*************************************************************************
    TEST(test_pair_copy_construct)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2(p1);

      CHECK_EQUAL(p1.first,  p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_move_construct)
    {
      etl::pair<ItemM1, ItemM2> p1(1, 2.3);
      etl::pair<ItemM1, ItemM2> p2(std::move(p1));

      CHECK(!bool(p1.first));
      CHECK(!bool(p1.second));

      CHECK_EQUAL(1,   p2.first.value);
      CHECK_EQUAL(2.3, p2.second.value);
    }

    //*************************************************************************
    TEST(test_pair_copy_construct_alternate)
    {
      etl::pair<char, float> p1(1, 2.3f);
      etl::pair<int, double> p2(p1);

      CHECK_EQUAL(p1.first,  p2.first);
      CHECK_EQUAL(p1.second, p2.second);
    }

    //*************************************************************************
    TEST(test_make_pair)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2;
      p2 = etl::make_pair(1, 2.3);

      CHECK_EQUAL(p1.first,  p2.first);
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

      CHECK_EQUAL(2,   p1.first);
      CHECK_EQUAL(3.4, p1.second);

      CHECK_EQUAL(1,   p2.first);
      CHECK_EQUAL(2.3, p2.second);
    }

    //*************************************************************************
    TEST(test_pair_swap_global)
    {
      etl::pair<int, double> p1(1, 2.3);
      etl::pair<int, double> p2(2, 3.4);

      swap(p1, p2);

      CHECK_EQUAL(2,   p1.first);
      CHECK_EQUAL(3.4, p1.second);

      CHECK_EQUAL(1,   p2.first);
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
      int a = 1;
      const int b = 2;
      int c = etl::exchange(a, b); // c = a, a = b

      CHECK_EQUAL(2, a);
      CHECK_EQUAL(2, b);
      CHECK_EQUAL(1, c);
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
      constCalled = false;

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

      const Map map = {{1, 0.3},
                       {47, 0.8},
                       {33, 0.1}};
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

      const Map map = {{1, 0.3},
                       {47, 0.8},
                       {33, 0.1}};
      Vector    result{};

      // Extract the map values into a vector
      std::transform(map.begin(), map.end(), std::back_inserter(result), etl::select2nd<Map::value_type>());
      //! [test_select2nd_example]

      CHECK_EQUAL(3, result.size());

      const Vector expected{0.1, 0.3, 0.8};
      sort(result.begin(), result.end());  // sort for comparison
      CHECK_ARRAY_CLOSE(expected, result, 3, 0.0001);
    }

    //*************************************************************************
    TEST(test_functor)
    {
      constexpr etl::functor<int, int> fw1(TestGlobal);
      CHECK_EQUAL(2, fw1(1));
    }

    //*************************************************************************
    TEST(test_member_function_wrapper)
    {
      constexpr int(*pf)(int) = &etl::member_function_wrapper<int(int)>::function<TestClass, test, &TestClass::MemberFunction>;

      CHECK_EQUAL(2, pf(1));
    }

    //*************************************************************************
    TEST(test_functor_wrapper)
    {
      constexpr int(*pf)(int) = &etl::functor_wrapper<int(int)>::function<TestClass, test>;

      CHECK_EQUAL(2, pf(1));
    }

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
    std::ostream& operator << (std::ostream& os, forward_call_type type)
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
    forward_call_type template_function_f(T&& t)
    {
      return function_f(etl::forward<T>(t));
    }

    //*********************************
    TEST(test_forward)
    {
      SF s1;
      const SF s2;

      CHECK_EQUAL(forward_call_type::LValue,      template_function_f(s1));
      CHECK_EQUAL(forward_call_type::RValue,      template_function_f(etl::move(s1)));
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
    std::ostream& operator << (std::ostream& os, forward_like_call_type type)
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
    forward_like_call_type template_function_fl(U&& u)
    {
      return function_fl(etl::forward_like<T>(u));
    }

    //*********************************
    TEST(test_forward_like)
    {
      UFL u1;
      const UFL u2;
      UFL& u3 = u1;
      const UFL& u4 = u2;

      CHECK_EQUAL(forward_like_call_type::LValue,      template_function_fl<TFL&>(u1));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(u1));
      CHECK_EQUAL(forward_like_call_type::RValue,      template_function_fl<TFL&&>(u1));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(u1));

      CHECK_EQUAL(forward_like_call_type::LValue,      template_function_fl<TFL&>(etl::move(u1)));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(etl::move(u1)));
      CHECK_EQUAL(forward_like_call_type::RValue,      template_function_fl<TFL&&>(etl::move(u1)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(etl::move(u1)));

      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<TFL&>(u2));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(u2));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<TFL&&>(u2));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(u2));

      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<TFL&>(etl::move(u2)));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(etl::move(u2)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<TFL&&>(etl::move(u2)));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(etl::move(u2)));

      CHECK_EQUAL(forward_like_call_type::LValue,      template_function_fl<TFL&>(u3));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(u3));
      CHECK_EQUAL(forward_like_call_type::RValue,      template_function_fl<TFL&&>(u3));
      CHECK_EQUAL(forward_like_call_type::ConstRValue, template_function_fl<const TFL&&>(u3));

      CHECK_EQUAL(forward_like_call_type::LValue,      template_function_fl<TFL&>(etl::move(u3)));
      CHECK_EQUAL(forward_like_call_type::ConstLValue, template_function_fl<const TFL&>(etl::move(u3)));
      CHECK_EQUAL(forward_like_call_type::RValue,      template_function_fl<TFL&&>(etl::move(u3)));
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
  };
}
