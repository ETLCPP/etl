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

#include "etl/functional.h"

#include <list>
#include <vector>
#include <numeric>

namespace
{
  template <typename TCompare>
  bool compare(int a, int b)
  {
    return TCompare()(a, b);
  }

  struct test : etl::binary_function<int, int, bool>
  {
    bool operator()(int a, int b) const
    {
      return a < b;
    }
  };

  struct MemFnTest
  {
    std::string Function1()
    {
      return std::string("Function1");
    }

    std::string Function2(const std::string& arg1)
    {
      return std::string("Function2: ") + arg1;
    }

    std::string Function3(const std::string& arg1, const std::string& arg2)
    {
      return std::string("Function3: ") + arg1 + arg2;
    }

    std::string Function1_Const() const
    {
      return std::string("Function1_Const");
    }

    std::string Function2_Const(const std::string& arg1) const
    {
      return std::string("Function2_Const: ") + arg1;
    }

    std::string Function3_Const(const std::string& arg1, const std::string& arg2) const
    {
      return std::string("Function3_Const: ") + arg1 + arg2;
    }

    mutable std::string result;
  };

  SUITE(test_functional)
  {
    //*************************************************************************
    TEST(test_less)
    {
      CHECK((compare<etl::less<int>>(1, 2)));
      CHECK(!(compare<etl::less<int>>(2, 1)));
      CHECK(!(compare<etl::less<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_less_equal)
    {
      CHECK((compare<etl::less_equal<int>>(1, 2)));
      CHECK(!(compare<etl::less_equal<int>>(2, 1)));
      CHECK((compare<etl::less_equal<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_greater)
    {
      CHECK(!(compare<etl::greater<int>>(1, 2)));
      CHECK((compare<etl::greater<int>>(2, 1)));
      CHECK(!(compare<etl::greater<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_greater_equal)
    {
      CHECK(!(compare<etl::greater_equal<int>>(1, 2)));
      CHECK((compare<etl::greater_equal<int>>(2, 1)));
      CHECK((compare<etl::greater_equal<int>>(1, 1)));
    }

    //*************************************************************************
    TEST(test_equal_to)
    {
      CHECK((compare<etl::equal_to<int>>(1, 1)));
      CHECK(!(compare<etl::equal_to<int>>(1, 2)));
      CHECK(!(compare<etl::equal_to<int>>(2, 1)));
    }

    //*************************************************************************
    TEST(test_not_equal_to)
    {
      CHECK(!(compare<etl::not_equal_to<int>>(1, 1)));
      CHECK((compare<etl::not_equal_to<int>>(1, 2)));
      CHECK((compare<etl::not_equal_to<int>>(2, 1)));
    }

    //*************************************************************************
    TEST(test_bind1st)
    {
      CHECK((etl::bind1st(test(), 1)(2)));
      CHECK(!(etl::bind1st(test(), 2)(1)));
    }

    //*************************************************************************
    TEST(test_bind2nd)
    {
      CHECK(!(etl::bind2nd(test(), 1)(2)));
      CHECK((etl::bind2nd(test(), 2)(1)));
    }

    //*************************************************************************
    TEST(test_reference_wrapper)
    {
      int a = 0;
      etl::reference_wrapper<int> ra(a);

      ra.get() = 1;
      CHECK_EQUAL(1, a);
      CHECK_EQUAL(1, ra);

      a = 2;
      CHECK_EQUAL(2, a);
      CHECK_EQUAL(2, ra);
    }

    //*************************************************************************
    TEST(test_reference_wrapper_container)
    {
      std::list<int> test    = { 0, 1, 2, 3, 4 };
      std::list<int> compare = { 0, 1, 2, 3, 4 };
      std::vector<etl::reference_wrapper<int>> test_ref(test.begin(), test.end());

      std::list<int>::const_iterator itest    = test.begin();
      std::list<int>::const_iterator icompare = compare.begin();

      while (icompare != compare.end())
      {
        CHECK_EQUAL(*icompare, *itest);
        ++itest;
        ++icompare;
      }
    }

    //*************************************************************************
    TEST(test_ref)
    {
      int a = 0;
      etl::reference_wrapper<int> ra = etl::ref(a);

      ra.get() = 1;
      CHECK_EQUAL(1, a);
      CHECK_EQUAL(1, ra);
    }

    //*************************************************************************
    TEST(test_cref)
    {
      int a = 0;
      etl::reference_wrapper<const int> ra = etl::cref(a);

      a = 1;
      CHECK_EQUAL(1, a);
      CHECK_EQUAL(1, ra);
    }

    //*************************************************************************
    TEST(test_plus)
    {
      auto f = etl::plus<int>();
      CHECK_EQUAL(2 + 4, f(2, 4));
    }

    //*************************************************************************
    TEST(test_minus)
    {
      auto f = etl::minus<int>();
      CHECK_EQUAL(2 - 4, f(2, 4));
    }

    //*************************************************************************
    TEST(test_negate)
    {
      auto f = etl::negate<int>();
      CHECK_EQUAL(-2, f(2));
    }

    //*************************************************************************
    TEST(test_multiplies)
    {
      auto f = etl::multiplies<int>();
      CHECK_EQUAL(2 * 4, f(2, 4));
    }

    //*************************************************************************
    TEST(test_divides)
    {
      auto f = etl::divides<int>();
      CHECK_EQUAL(4 / 2, f(4, 2));
    }

    //*************************************************************************
    TEST(test_modulus)
    {
      auto f = etl::modulus<int>();
      CHECK_EQUAL(5 % 2, f(5, 2));
    }

    //*************************************************************************
    TEST(test_logical_and)
    {
      auto f = etl::logical_and<bool>();
      CHECK_EQUAL(false && false, f(false, false));
      CHECK_EQUAL(false && true,  f(false, true));
      CHECK_EQUAL(true  && false, f(true,  false));
      CHECK_EQUAL(true &&  true,  f(true,  true));
    }

    //*************************************************************************
    TEST(test_logical_or)
    {
      auto f = etl::logical_or<bool>();
      CHECK_EQUAL(false || false, f(false, false));
      CHECK_EQUAL(false || true,  f(false, true));
      CHECK_EQUAL(true  || false, f(true, false));
      CHECK_EQUAL(true  || true,  f(true, true));
    }

    //*************************************************************************
    TEST(test_logical_not)
    {
      auto f = etl::logical_not<bool>();
      CHECK_EQUAL(!false, f(false));
      CHECK_EQUAL(!true, f(true));
    }

    //*************************************************************************
    TEST(test_bit_and)
    {
      auto f = etl::bit_and<uint8_t>();
      CHECK_EQUAL(0x00U & 0xFFU, f(0x00U, 0xFFU));
      CHECK_EQUAL(0xAAU & 0xFFU, f(0xAAU, 0xFFU));
      CHECK_EQUAL(0x55U & 0xFFU, f(0x55U, 0xFFU));
      CHECK_EQUAL(0xFFU & 0xFFU, f(0xFFU, 0xFFU));
    }

    //*************************************************************************
    TEST(test_bit_or)
    {
      auto f = etl::bit_or<uint8_t>();
      CHECK_EQUAL(0xFFU | 0x00U, f(0xFFU, 0x00U));
      CHECK_EQUAL(0xAAU | 0x00U, f(0xAAU, 0x00U));
      CHECK_EQUAL(0x55U | 0x00U, f(0x55U, 0x00U));
      CHECK_EQUAL(0x55U | 0xAAU, f(0x55U, 0xAAU));
    }

    //*************************************************************************
    TEST(test_bit_xor)
    {
      auto f = etl::bit_xor<uint8_t>();
      CHECK_EQUAL(0xFFU ^ 0x00U, f(0xFFU, 0x00U));
      CHECK_EQUAL(0xAAU ^ 0x00U, f(0xAAU, 0x00U));
      CHECK_EQUAL(0x55U ^ 0x00U, f(0x55U, 0x00U));
      CHECK_EQUAL(0x55U ^ 0xAAU, f(0x55U, 0xAAU));
    }

    //*************************************************************************
    TEST(test_bit_not)
    {
      auto f = etl::bit_not<uint8_t>();
      CHECK_EQUAL(uint8_t(~0x00U), f(0x00U));
      CHECK_EQUAL(uint8_t(~0x55U), f(0x55U));
      CHECK_EQUAL(uint8_t(~0xAAU), f(0xAAU));
      CHECK_EQUAL(uint8_t(~0xFFU), f(0xFFU));
    }

    //*************************************************************************
    TEST(test_mem_fn)
    {
      MemFnTest mft;
      std::string result;

      auto f1 = etl::mem_fn(&MemFnTest::Function1);
      result.clear();
      result = f1(mft);
      CHECK_EQUAL("Function1", result);

      auto f2 = etl::mem_fn(&MemFnTest::Function2);
      result.clear();
      result = f2(mft, "Arg1");
      CHECK_EQUAL("Function2: Arg1", result);

      auto f3 = etl::mem_fn(&MemFnTest::Function3);
      result.clear();
      result = f3(mft, "Arg1", " : Arg2");
      CHECK_EQUAL("Function3: Arg1 : Arg2", result);
    }

    //*************************************************************************
    TEST(test_const_mem_fn)
    {
      const MemFnTest mft;
      std::string result;

      auto f1 = etl::mem_fn(&MemFnTest::Function1_Const);
      result.clear();
      result = f1(mft);
      CHECK_EQUAL("Function1_Const", result);

      auto f2 = etl::mem_fn(&MemFnTest::Function2_Const);
      result.clear();
      result = f2(mft, "Arg1");
      CHECK_EQUAL("Function2_Const: Arg1", result);

      auto f3 = etl::mem_fn(&MemFnTest::Function3_Const);
      result.clear();
      result = f3(mft, "Arg1", " : Arg2");
      CHECK_EQUAL("Function3_Const: Arg1 : Arg2", result);
    }

    //*************************************************************************
    TEST(test_constexpr_mem_fn)
    {
      const MemFnTest mft;
      std::string result;

      constexpr auto f1 = etl::mem_fn(&MemFnTest::Function1_Const);
      result.clear();
      result = f1(mft);
      CHECK_EQUAL("Function1_Const", result);

      constexpr auto f2 = etl::mem_fn(&MemFnTest::Function2_Const);
      result.clear();
      result = f2(mft, "Arg1");
      CHECK_EQUAL("Function2_Const: Arg1", result);

      constexpr auto f3 = etl::mem_fn(&MemFnTest::Function3_Const);
      result.clear();
      result = f3(mft, "Arg1", " : Arg2");
      CHECK_EQUAL("Function3_Const: Arg1 : Arg2", result);
    }
  };
}
