/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#include "etl/expected.h"
#include "etl/type_traits.h"

#include <string>
#include <vector>

namespace
{
  struct Value
  {
    std::string v;

    Value()
    {
    }

    Value(const std::string v_)
      : v(v_)
    {
    }

    operator std::string() const
    {
      return v;
    }
  };

  struct ValueM
  {
    ValueM()
    {
    }

    ValueM(const std::string& v_) 
      : v(v_)
    {
    }

    ValueM(ValueM&&) = default;
    ValueM& operator =(ValueM&&) = default;

    ValueM(const ValueM&) = delete;
    ValueM& operator =(const ValueM&) = delete;

    std::string v;
  };

  struct Error
  {
    std::string e;

    Error() = default;
    Error(const Error&) = default;
    Error& operator =(const Error&) = default;

    Error(const std::string e_)
      : e(e_)
    {
    }

    Error& operator =(const std::string e_)
    {
      e = e_;
      return *this;
    }

    operator std::string() const
    {
      return e;
    }
  };

  struct ErrorM
  {
    ErrorM()
    {
    }

    ErrorM(const std::string& e_)
      : e(e_)
    {
    }

    operator std::string() const
    {
      return e;
    }

    ErrorM(ErrorM&&) = default;
    ErrorM& operator =(ErrorM&&) = default;

    ErrorM(const ErrorM&) = delete;
    ErrorM& operator =(const ErrorM&) = delete;

    std::string e;
  };

  using Expected   = etl::expected<Value,  Error>;
  using ExpectedV  = etl::expected<void,   Error>;
  using ExpectedM  = etl::expected<ValueM, ErrorM>;
  using ExpectedVM = etl::expected<void,   ErrorM>;

  using Unexpected  = etl::unexpected<Error>;
  using UnexpectedV = etl::unexpected<Error>;
  using UnexpectedM = etl::unexpected<ErrorM>;
}

namespace
{
  SUITE(test_expected)
  {
    //*************************************************************************
    TEST(test_default_constructor)
    {
      Expected expected;

      CHECK_TRUE(expected.has_value());
      CHECK_TRUE(bool(expected));
    }

    //*************************************************************************
    TEST(test_constructor_for_result_with_value)
    {
      Value    input = { "value 1" };
      Expected expected(input);

      Value output = expected.value();

      CHECK_TRUE(expected.has_value());
      CHECK_TRUE(bool(expected));
      CHECK_TRUE(output.v == input.v);
    }

    //*************************************************************************
    TEST(test_constructor_for_const_result_with_value)
    {
      Value          input = { "value 1" };
      const Expected expected(input);

      const Value& output = expected.value();

      CHECK_TRUE(expected.has_value());
      CHECK_TRUE(bool(expected));
      CHECK_TRUE(output.v == input.v);
    }

    //*************************************************************************
    TEST(test_constructor_for_moveable_result_with_value)
    {
      ValueM    input = { "value 1" };
      ExpectedM expected(etl::move(input));

      ValueM output = etl::move(expected.value());

      CHECK_TRUE(expected.has_value());
      CHECK_TRUE(bool(expected));
      CHECK_TRUE(output.v == std::string("value 1"));
    }

    //*************************************************************************
    TEST(test_constructor_for_result_with_error)
    {
      Error      input = { "error 1" };

      Unexpected unexpected(input);
      Expected   expected(unexpected);

      Error output = expected.error();

      CHECK_FALSE(expected.has_value());
      CHECK_FALSE(bool(expected));
      CHECK(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_constructor_for_const_result_with_error)
    {
      const Error      input = { "error 1" };
      const Unexpected unexpected(input);
      const Expected   expected(unexpected);

      const Error& output = expected.error();

      CHECK_FALSE(expected.has_value());
      CHECK_FALSE(bool(expected));
      CHECK(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_constructor_for_moveable_result_with_error)
    {
      ErrorM      input = { "error 1" };
      UnexpectedM unexpected(etl::move(input));
      ExpectedM   expected(etl::move(unexpected));

      ErrorM output = etl::move(expected.error());

      CHECK_FALSE(expected.has_value());
      CHECK_FALSE(bool(expected));
      CHECK_TRUE(output.e == std::string("error 1"));
    }

    //*************************************************************************
    TEST(test_constructor_for_result_void_value_with_value)
    {
      ExpectedV expected;

      CHECK_TRUE(expected.has_value());
      CHECK_TRUE(bool(expected));
    }

    //*************************************************************************
    TEST(test_constructor_for_const_result_void_value_with_value)
    {
      const ExpectedV expected;

      CHECK_TRUE(expected.has_value());
      CHECK_TRUE(bool(expected));
    }

    //*************************************************************************
    TEST(test_constructor_for_result_void_value_with_error)
    {
      Error       input = { "error 1" };
      UnexpectedV unexpected(input);
      ExpectedV   expected(unexpected);

      Error output = expected.error();

      CHECK_FALSE(expected.has_value());
      CHECK_FALSE(bool(expected));
      CHECK_TRUE(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_constructor_for_const_result_void_value_with_error)
    {
      const Error       input = { "error 1" };
      const UnexpectedV unexpected(input);
      const ExpectedV   expected(unexpected);

      const Error& output = expected.error();

      CHECK_FALSE(expected.has_value());
      CHECK_FALSE(bool(expected));
      CHECK_TRUE(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_constructor_for_moveable_result_void_value_with_error)
    {
      ErrorM      input = { "error 1" };
      UnexpectedM unexpected(etl::move(input));
      ExpectedM   expected(etl::move(unexpected));

      ErrorM output = etl::move(expected.error());

      CHECK_FALSE(expected.has_value());
      CHECK_FALSE(bool(expected));
      CHECK(output.e == std::string("error 1"));
    }

    //*************************************************************************
    TEST(test_copy_construct)
    {
      Value    input1 = { "value 1" };
      Expected expected1(input1);
      Expected expected2(expected1);

      Value output1 = expected2.value();

      CHECK_TRUE(expected1.has_value());
      CHECK_TRUE(expected2.has_value());
      CHECK(output1.v == input1.v);
    }

    //*************************************************************************
    TEST(test_copy_assign)
    {
      Value    input1 = { "value 1" };
      Expected expected1(input1);

      Value    input2 = { "value 2" };
      Expected expected2(input2);

      expected2 = expected1;

      Value output1 = expected1.value();
      Value output2 = expected2.value();

      CHECK_TRUE(expected1.has_value());
      CHECK_TRUE(expected2.has_value());

      //CHECK(output1.v != input1.v);
      //CHECK(output2.v != input1.v);

      //CHECK(output1.v == input2.v);
      //CHECK(output2.v == input2.v);
    }

    //*************************************************************************
    TEST(test_copy_assign_from_error)
    {
      Value    input = { "value 1" };
      Expected expected(input);

      Error      error = { "error 1" };
      Unexpected unexpected(error);

      expected = unexpected;

      Error output = expected.error();

      CHECK_FALSE(expected.has_value());
      CHECK_EQUAL(std::string(error), std::string(expected.error()));
    }

    //*************************************************************************
    TEST(test_move_construct)
    {
      ValueM    input1 = { "value 1" };
      ExpectedM expected1(etl::move(input1));
      ExpectedM expected2(etl::move(expected1));

      ValueM output1 = etl::move(expected1.value());
      ValueM output2 = etl::move(expected2.value());

      CHECK_TRUE(expected1.has_value());
      CHECK_TRUE(expected2.has_value());

      CHECK_EQUAL("", output1.v);
      CHECK_EQUAL("value 1", output2.v);
    }

    //*************************************************************************
    TEST(test_move_assign)
    {
      ValueM    input1 = { "value 1" };
      ExpectedM expected1(etl::move(input1));

      ValueM    input2 = { "value 2" };
      ExpectedM expected2(etl::move(input2));

      expected2 = etl::move(expected1);

      ValueM output1 = etl::move(expected1.value());
      ValueM output2 = etl::move(expected2.value());

      CHECK_TRUE(expected1.has_value());
      CHECK_TRUE(expected2.has_value());

      CHECK_EQUAL("", output1.v);
      CHECK_EQUAL("value 1", output2.v);
    }

    //*************************************************************************
    TEST(test_move_assign_from_error)
    {
      ValueM    input = { "value 1" };
      ExpectedM expected(etl::move(input));

      ErrorM      error = { "error 1" };
      UnexpectedM unexpected(etl::move(error));

      expected = etl::move(unexpected);

      ErrorM output = etl::move(expected.error());

      CHECK_FALSE(expected.has_value());
      CHECK_EQUAL(std::string(error), std::string(expected.error()));
    }

    //*************************************************************************
    TEST(test_copy_construct_void_value)
    {
      Error       input1 = { "error 1" };
      UnexpectedV unexpected1(input1);
      ExpectedV   expected1(unexpected1);
      ExpectedV   expected2(expected1);

      CHECK_FALSE(expected1.has_value());
      CHECK_FALSE(expected2.has_value());

      Error output1 = expected1.error();
      Error output2 = expected2.error();

      CHECK_EQUAL(input1.e, output1.e);
      CHECK_EQUAL(input1.e, output2.e);
    }

    //*************************************************************************
    TEST(test_copy_assign_void_value)
    {
      Error       input1 = { "error 1" };
      UnexpectedV unexpected1(input1);

      Error       input2 = { "error 2" };
      UnexpectedV unexpected2(input2);

      ExpectedV   expected1(unexpected1);
      ExpectedV   expected2(unexpected2);

      expected2 = expected1;

      Error output1 = expected1.error();
      Error output2 = expected2.error();

      CHECK_FALSE(expected1.has_value());
      CHECK_FALSE(expected2.has_value());

      CHECK_EQUAL(input1.e, output1.e);
      CHECK_EQUAL(input1.e, output2.e);
    }

    //*************************************************************************
    TEST(test_emplace_from_initializer_list)
    {
      struct S
      {
        S()
          : vi()
          , a(0)
          , b(0)
        {
        }

        S(std::initializer_list<int> il, int a_, int b_)
          : vi(il)
          , a(a_)
          , b(b_)
        {
        }

        std::vector<int> vi;
        int a;
        int b;
      };

      etl::expected<S, Error> exp;

      S s1({ 10, 11, 12 }, 1, 2);
      S s2 = exp.emplace({ 10, 11, 12 }, 1, 2);
      
      CHECK(s1.vi == s2.vi);
      CHECK_EQUAL(s1.a, s2.a);
      CHECK_EQUAL(s1.b, s2.b);
    }

    //*************************************************************************
    TEST(test_emplace_from_vargs)
    {
      struct S
      {
        S()
          : a(0)
          , b(0)
        {
        }

        S(int a_, int b_)
          : a(a_)
          , b(b_)
        {
        }

        int a;
        int b;
      };

      etl::expected<S, Error> exp;

      S s1(1, 2);
      S s2 = exp.emplace(1, 2);

      CHECK_EQUAL(s1.a, s2.a);
      CHECK_EQUAL(s1.b, s2.b);
    }

    //*************************************************************************
    TEST(test_move_construct_void_value)
    {
      ErrorM      input1 = { "error 1" };
      UnexpectedM unexpected1(etl::move(input1));
      ExpectedVM  expected1(etl::move(unexpected1));
      ExpectedVM  expected2(etl::move(expected1));

      ErrorM output1 = etl::move(expected1.error());
      ErrorM output2 = etl::move(expected2.error());

      CHECK_FALSE(expected1.has_value());
      CHECK_FALSE(expected2.has_value());

      CHECK_EQUAL("",        output1.e);
      CHECK_EQUAL("error 1", output2.e);
    }

    //*************************************************************************
    TEST(test_move_assign_void_value)
    {
      ErrorM      input1 = { "error 1" };
      UnexpectedM unexpected1(etl::move(input1));

      ErrorM      input2 = { "error 2" };
      UnexpectedM unexpected2(etl::move(input2));

      ExpectedVM  expected1(etl::move(unexpected1));
      ExpectedVM  expected2(etl::move(unexpected2));

      expected2 = etl::move(expected1);

      ErrorM output1 = etl::move(expected1.error());
      ErrorM output2 = etl::move(expected2.error());

      CHECK_FALSE(expected1.has_value());
      CHECK_FALSE(expected2.has_value());

      CHECK_EQUAL("",        output1.e);
      CHECK_EQUAL("error 1", output2.e);
    }

    //*************************************************************************
    TEST(test_dereference_operators)
    {
      struct ExpectedType
      {
        ExpectedType(int i_)
          : i(i_)
        {
        }

        int i;
      };

      etl::expected<ExpectedType, int>       exp  = etl::unexpected<int>(0);
      const etl::expected<ExpectedType, int> cexp = etl::unexpected<int>(0);
    
      CHECK_THROW({ int i = (*exp).i;  (void)i; }, etl::expected_invalid);
      CHECK_THROW({ int i = (*cexp).i; (void)i; }, etl::expected_invalid);
      CHECK_THROW({ int i = exp->i;    (void)i; }, etl::expected_invalid);
      CHECK_THROW({ int i = cexp->i;   (void)i; }, etl::expected_invalid);
    }

    //*************************************************************************
    struct value_or_helper
    {
      Expected get_value() const
      {
        Value value = { "value5" };
        return Expected(value);
      }

      Expected get_error() const
      {
        Error error = { "error1" };
        return Expected(Unexpected(error));
      }
    };

    TEST(test_chained_value_or_github_bug_720)
    {
      value_or_helper helper{};

      Value value1 = helper.get_value().value_or(Value("value1"));
      CHECK_EQUAL("value5", value1.v);

      Value value2 = helper.get_error().value_or(Value("value1"));
      CHECK_EQUAL("value1", value2.v);
    }

    //*************************************************************************
    TEST(test_assignment_bug_738)
    {
      etl::expected<int, int> test_exp{1};
      CHECK_TRUE(test_exp.has_value());

      test_exp = etl::unexpected<int>(2);
      CHECK_FALSE(test_exp.has_value());
    }

    //*************************************************************************
    TEST(test_expected_does_not_compile_with_ETL_LOG_ERRORS_bug_787)
    {
      etl::expected<int, int> test_exp = etl::unexpected<int>(0);
      bool thrown = false;
      std::string thrown_what;
      std::string exception_what = etl::expected_invalid(__FILE__, __LINE__).what();

      try
      {
        int i = *test_exp;
        (void)i;
      }
      catch (etl::exception& e)
      {
        thrown = true;
        thrown_what = e.what(); // what() should be accessible
      }

      CHECK_TRUE(thrown);
      CHECK_TRUE(exception_what == thrown_what);
    }
  };
}
