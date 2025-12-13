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
    TEST(test_constructor_in_place_result_with_value)
    {
      struct ValueInPlace
      { 
        ValueInPlace()
          : a(0)
          , b(0)
        {
        }

        ValueInPlace(int a_, int b_)
          : a(a_)
          , b(b_)
        {
        }

        int a;
        int b;
      };

      using ExpectedInPlace = etl::expected<ValueInPlace,  Error>;

      ExpectedInPlace expected(etl::in_place_t(), 1, 2);

      ValueInPlace output = expected.value();

      CHECK_TRUE(expected.has_value());
      CHECK_TRUE(bool(expected));
      CHECK_TRUE(output.a == 1);
      CHECK_TRUE(output.b == 2);
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

    //*************************************************************************
    TEST(test_expected_equal_operator)
    {
      etl::expected<int, int> test_exp = 1;
      etl::expected<int, int> test_exp_equal = 1;
      etl::expected<int, int> test_exp_unequal = 2;
      int test_val_equal = 1;
      int test_val_unequal = 2;
      etl::expected<int, int> test_unexp = etl::unexpected<int>(1);
      etl::expected<int, int> test_unexp_equal = etl::unexpected<int>(1);
      etl::expected<int, int> test_unexp_unequal = etl::unexpected<int>(2);

      CHECK_TRUE(test_exp == test_exp_equal);
      CHECK_FALSE(test_exp != test_exp_equal);

      CHECK_FALSE(test_exp == test_exp_unequal);
      CHECK_TRUE(test_exp != test_exp_unequal);

      CHECK_TRUE(test_exp == test_val_equal);
      CHECK_FALSE(test_exp != test_val_equal);

      CHECK_FALSE(test_exp == test_val_unequal);
      CHECK_TRUE(test_exp != test_val_unequal);

      CHECK_FALSE(test_exp == test_unexp);
      CHECK_TRUE(test_exp != test_unexp);

      CHECK_TRUE(test_unexp == test_unexp_equal);
      CHECK_FALSE(test_unexp != test_unexp_equal);

      CHECK_FALSE(test_unexp == test_unexp_unequal);
      CHECK_TRUE(test_unexp != test_unexp_unequal);
    }


    //*************************************************************************
    TEST(test_expected_void_equal_operator)
    {
      etl::expected<void, int> test_exp;
      etl::expected<void, int> test_exp2;
      etl::expected<void, int> test_unexp = etl::unexpected<int>(1);
      etl::expected<void, int> test_unexp2 = etl::unexpected<int>(2);

      CHECK_TRUE(test_exp == test_exp2);
      CHECK_FALSE(test_exp != test_exp2);

      CHECK_FALSE(test_exp == test_unexp);
      CHECK_TRUE(test_exp != test_unexp);

      CHECK_FALSE(test_unexp == test_unexp2);
      CHECK_TRUE(test_unexp != test_unexp2);
    }

    //*************************************************************************
    TEST(test_unexpected_equal_operator)
    {
      etl::unexpected<int> test_unexp = etl::unexpected<int>(1);
      etl::unexpected<int> test_unexp_equal = etl::unexpected<int>(1);
      etl::unexpected<int> test_unexp_unequal = etl::unexpected<int>(2);

      CHECK_TRUE(test_unexp == test_unexp_equal);
      CHECK_FALSE(test_unexp != test_unexp_equal);

      CHECK_FALSE(test_unexp == test_unexp_unequal);
      CHECK_TRUE(test_unexp != test_unexp_unequal);
    }

    //*************************************************************************
    TEST(test_expected_swap)
    {
      etl::expected<int, int> test_exp_1 = 1;
      etl::expected<int, int> test_exp_2 = 2;
      etl::expected<int, int> test_unexp_1 = etl::unexpected<int>(1);
      etl::expected<int, int> test_unexp_2 = etl::unexpected<int>(2);

      etl::expected<int, int> test_exp_1_swap = test_exp_1;
      etl::expected<int, int> test_exp_2_swap = test_exp_2;

      swap(test_exp_1_swap, test_exp_2_swap);

      CHECK_TRUE(test_exp_1_swap == test_exp_2);
      CHECK_TRUE(test_exp_2_swap == test_exp_1);

      etl::expected<int, int> test_unexp_1_swap = test_unexp_1;
      etl::expected<int, int> test_unexp_2_swap = test_unexp_2;

      swap(test_unexp_1_swap, test_unexp_2_swap);

      CHECK_TRUE(test_unexp_1_swap == test_unexp_2);
      CHECK_TRUE(test_unexp_2_swap == test_unexp_1);

      etl::expected<int, int> test_exp_swap = test_exp_1;
      etl::expected<int, int> test_unexp_swap = test_unexp_1;

      swap(test_exp_swap, test_unexp_swap);

      CHECK_TRUE(test_exp_swap == test_unexp_1);
      CHECK_TRUE(test_unexp_swap == test_exp_1);
    }

    //*************************************************************************
    TEST(test_expected_void_swap)
    {
      etl::expected<void, int> test_exp;
      etl::expected<void, int> test_unexp = etl::unexpected<int>(1);

      etl::expected<void, int> test_exp_swap = test_exp;
      etl::expected<void, int> test_unexp_swap = test_unexp;

      swap(test_exp_swap, test_unexp_swap);

      CHECK_TRUE(test_exp_swap == test_unexp);
      CHECK_TRUE(test_unexp_swap == test_exp);
    }

    //*************************************************************************
    TEST(test_unexpected_swap)
    {
      etl::unexpected<int> test_unexp_1 = etl::unexpected<int>(1);
      etl::unexpected<int> test_unexp_2 = etl::unexpected<int>(2);

      etl::unexpected<int> test_unexp_1_swap = test_unexp_1;
      etl::unexpected<int> test_unexp_2_swap = test_unexp_2;

      swap(test_unexp_1_swap, test_unexp_2_swap);

      CHECK_TRUE(test_unexp_1_swap == test_unexp_2);
      CHECK_TRUE(test_unexp_2_swap == test_unexp_1);
    }

    //*************************************************************************
    template <typename TValue, typename TExpected, typename Enable = void>
    struct value_type_helper {
      static bool check(TExpected& expected) {
        return etl::is_same<
          typename etl::decay<decltype(expected.value())>::type, 
          TValue
        >::value;
      }
    };

    template <typename TValue, typename TExpected>
    struct value_type_helper<TValue, TExpected, typename etl::enable_if<etl::is_void<TValue>::value>::type> {
      static bool check(TExpected& expected) {
        (void)expected;
        return true;
      }
    };

    template <typename TValue, typename TError, typename TExpected>
    bool check_expected_type_helper(TExpected& expected) {

        bool value_type_ok = value_type_helper<TValue, TExpected>::check(expected);

        bool error_type_ok = etl::is_same<
            typename etl::decay<decltype(expected.error())>::type, 
            TError
        >::value;

        bool expected_type_ok = etl::is_same<
            typename etl::decay<decltype(expected)>::type, 
            etl::expected<TValue, TError>
        >::value;

        return value_type_ok && error_type_ok && expected_type_ok;
    }

    //*************************************************************************

    TEST(test_or_else) {
      Expected expected = {Value("or_else_with_value")};
      Expected expected_error = {Unexpected(Error("or_else_with_error"))};

      const Expected expected_const = {Value("const_or_else_with_value")};
      const Expected expected_error_const = {Unexpected(Error("const_or_else_with_error"))};

      bool error_generated {false};

      auto expected_out = expected.or_else([&error_generated](Error e) -> Expected {
        error_generated = true;
        return Unexpected(e);
      });

      CHECK_FALSE(error_generated);
      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("or_else_with_value", expected_out.value().v);

      auto with_value_type_check = check_expected_type_helper<Value, Error>(expected_out);
      CHECK_TRUE(with_value_type_check);

      error_generated = false;
      auto expected_const_out = expected_const.or_else([&error_generated](const Error& e) -> Expected {
        error_generated = true;
        return Unexpected(e);
      });

      CHECK_FALSE(error_generated);
      CHECK_TRUE(expected_const_out.has_value());
      CHECK_EQUAL("const_or_else_with_value", expected_const_out.value().v);

      error_generated = false;
      auto unexpected_out = expected_error.or_else([&error_generated](Error e) -> Expected {
        error_generated = true;
        return Unexpected(e);
      });

      CHECK_TRUE(error_generated);
      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<Value,Error>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("or_else_with_error", unexpected_out.error().e);


      error_generated = false;
      auto unexpected_const_out = expected_error_const.or_else([&error_generated](const Error& e) -> Expected {
        error_generated = true;
        return Unexpected(e);
      });

      CHECK_TRUE(error_generated);
      CHECK_FALSE(unexpected_const_out.has_value());

      auto with_error_const_type_check = check_expected_type_helper<Value,Error>(unexpected_const_out);
      CHECK_TRUE(with_error_const_type_check);

      CHECK_EQUAL("const_or_else_with_error", unexpected_const_out.error().e);
    }

    //*************************************************************************

    TEST(test_or_else_move_constructor) {
      ExpectedM expected = ExpectedM(ValueM("or_else_with_value"));
      ExpectedM expected_error = ExpectedM(UnexpectedM(ErrorM("or_else_with_error")));
      bool error_generated {false};

      auto expected_out = etl::move(expected).or_else([&error_generated](ErrorM e) -> ExpectedM {
        error_generated = true;
        UnexpectedM unexpected(etl::move(e));
        return ExpectedM(etl::move(unexpected));
      });

      CHECK_FALSE(error_generated);
      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("or_else_with_value", expected_out.value().v);

      auto with_value_type_check = check_expected_type_helper<ValueM, ErrorM>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = etl::move(expected_error).or_else([&error_generated](ErrorM e) -> ExpectedM {
        error_generated = true;
        CHECK_EQUAL("or_else_with_error", e.e);

        UnexpectedM unexpected(etl::move(e));
        return ExpectedM(etl::move(unexpected));
      });

      CHECK_TRUE(error_generated);
      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<ValueM,ErrorM>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("or_else_with_error", unexpected_out.error().e);

      //The following should NOT compile. The const & overload should attempt to copy
      // const ExpectedM expected_error_const = ExpectedM(ValueM("or_else_with_value"));
      // expected_error_const.or_else([&error_generated](ErrorM e) -> const ExpectedM { 
      //   error_generated = true;
      //   UnexpectedM unexpected(etl::move(e));
      //   return ExpectedM(etl::move(unexpected));
      // });
    }

    //*************************************************************************

    TEST(test_or_else_void) {
      ExpectedV expected = ExpectedV();
      ExpectedV expected_error = ExpectedV(Unexpected(Error("or_else_with_error")));
      bool error_generated {false};

      auto expected_out = expected.or_else([&error_generated](Error e) -> ExpectedV {
        error_generated = true;
        return Unexpected(e);
      });

      CHECK_FALSE(error_generated);
      CHECK_TRUE(expected_out.has_value());

      auto with_value_type_check = check_expected_type_helper<void, Error>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = expected_error.or_else([&error_generated](Error e) -> ExpectedV {
        error_generated = true;
        CHECK_EQUAL("or_else_with_error", e.e);
        return Unexpected(e);
      });

      CHECK_TRUE(error_generated);
      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<void, Error>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("or_else_with_error", unexpected_out.error().e);
    }

    //*************************************************************************

    TEST(test_or_else_change_error) {
      Expected expected_error = {Unexpected(Error("or_else_with_error"))};
      ExpectedV expectedV_error = ExpectedV(Unexpected(Error("or_else_with_error")));

      auto change_to_string = expectedV_error.or_else([](Error e) -> etl::expected<void, std::string> {
        return etl::unexpected<std::string>(e.e.append("_to_string"));
      });

      auto with_error_type_check = check_expected_type_helper<void,std::string>(change_to_string);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("or_else_with_error_to_string", change_to_string.error());
    }

    //*************************************************************************

    TEST(test_or_else_change_error_move_constructor) {
      ExpectedM expected_error = ExpectedM(UnexpectedM(ErrorM("or_else_with_error")));

      auto change_to_string = etl::move(expected_error).or_else([](ErrorM e) -> etl::expected<ValueM, std::string> {
        return etl::unexpected<std::string>(e.e.append("_to_string"));
      });

      auto with_error_type_check = check_expected_type_helper<ValueM,std::string>(change_to_string);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("or_else_with_error_to_string", change_to_string.error());
    }
    
    //*************************************************************************

    TEST(test_or_else_const_rvalue) {
      bool error_generated {false};
      auto temp_expected = Expected(Unexpected(Error("temp_const_error")));

      auto unexpected_out = static_cast<const Expected&&>(temp_expected)
          .or_else([&error_generated](const Error& e) -> Expected {
          error_generated = true;
          CHECK_EQUAL("temp_const_error", e.e);
          return Expected(Unexpected(etl::move(e)));
      });
      
      CHECK_TRUE(error_generated);
      CHECK_EQUAL("temp_const_error", unexpected_out.error().e);
    }

    //*************************************************************************

    TEST(test_transform) {
      Expected expected = {Value("transform_with_value")};
      Expected expected_error = {Unexpected(Error("transform_with_error"))};
      const Expected expected_const = {Value("const_transform_with_value")};

      auto expected_out = expected.transform([](Value v) {
        auto s = v.v.append("_transformed");
        return s;
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("transform_with_value_transformed", expected_out.value());

      auto with_value_type_check = check_expected_type_helper<std::string, Error>(expected_out);
      CHECK_TRUE(with_value_type_check);


      auto expected_out_const = expected_const.transform([](const Value& v) {
        auto s = v;
        return s.v.append("_transformed");
      });

      CHECK_TRUE(expected_out_const.has_value());
      CHECK_EQUAL("const_transform_with_value_transformed", expected_out_const.value());

      auto const_with_value_type_check = check_expected_type_helper<std::string, Error>(expected_out_const);
      CHECK_TRUE(const_with_value_type_check);

      auto unexpected_out = expected_error.transform([](Value v) {
        auto s = v.v.append("_transformed");
        return s;
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<std::string,Error>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("transform_with_error", unexpected_out.error().e);
    }

    //*************************************************************************

    TEST(test_transform_move_constructor) {
      ExpectedM expected = {ValueM("transform_with_value")};
      ExpectedM expected_error = ExpectedM(UnexpectedM(ErrorM("transform_with_error")));

      auto expected_out = etl::move(expected).transform([](ValueM v) {
        auto s = v.v.append("_transformed");
        return etl::move(s);
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("transform_with_value_transformed", expected_out.value());

      auto with_value_type_check = check_expected_type_helper<std::string, ErrorM>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = etl::move(expected_error).transform([](ValueM v) {
        auto s = v.v.append("_transformed");
        return etl::move(s);
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<std::string,ErrorM>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("transform_with_error", unexpected_out.error().e);
    }

    //*************************************************************************

    TEST(test_transform_void) {
      ExpectedV expected;
      ExpectedV expected_error = {Unexpected(Error("transform_with_error"))};

      auto expected_out = expected.transform([]() {
        std::string s("_transformed");
        return s;
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("_transformed", expected_out.value());
      
      auto with_value_type_check = check_expected_type_helper<std::string, Error>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = expected_error.transform([]() {
        std::string s("_transformed");
        return s;
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<std::string,Error>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("transform_with_error", unexpected_out.error().e);
    }

      TEST(test_transform_void_move) {
      ExpectedVM expected;
      ExpectedVM expected_error = {UnexpectedM(ErrorM("transform_with_error"))};

      auto expected_out = etl::move(expected).transform([]() {
        std::string s("_transformed");
        return s;
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("_transformed", expected_out.value());
      
      auto with_value_type_check = check_expected_type_helper<std::string, ErrorM>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = etl::move(expected_error).transform([]() {
        std::string s("_transformed");
        return s;
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<std::string,ErrorM>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("transform_with_error", unexpected_out.error().e);
    }

    //*************************************************************************

    TEST(test_transform_to_void) {
      Expected expected {Value("transform_to_void")};
      
      bool executed {false};
      auto expected_out = expected.transform([&executed](Value v) {
        (void) v;
        executed = true;
        CHECK_EQUAL("transform_to_void", v.v);
        return;
      });

      auto to_void_type_check = check_expected_type_helper<void, Error>(expected_out);
      CHECK_TRUE(to_void_type_check);

      CHECK_TRUE(expected_out.has_value());
    }

    //*************************************************************************
    
    TEST(test_and_then) {
      Expected expected = {Value("and_then_with_value")};
      Expected expected_error = {Unexpected(Error("and_then_with_error"))};
      const Expected expected_const = {Value("const_and_then_with_value")};

      auto expected_out = expected.and_then([](Value v) -> Expected {
        return Value(v.v.append("_and_thened"));
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("and_then_with_value_and_thened", expected_out.value().v);

      auto with_value_type_check = check_expected_type_helper<Value, Error>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto expected_out_const = expected_const.and_then([](const Value& v) -> Expected {
        auto s = v;
        return Value(s.v.append("_and_thened"));
      });

      CHECK_TRUE(expected_out_const.has_value());
      CHECK_EQUAL("const_and_then_with_value_and_thened", expected_out_const.value().v);

      auto const_with_value_type_check = check_expected_type_helper<Value, Error>(expected_out_const);
      CHECK_TRUE(const_with_value_type_check);

      auto unexpected_out = expected_error.and_then([](Value v) -> Expected {
        return Value(v.v.append("_and_thened"));
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<Value,Error>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("and_then_with_error", unexpected_out.error().e);
    }
  
    //*************************************************************************

    TEST(test_and_then_move_constructor) {
      ExpectedM expected = ExpectedM(ValueM("and_then_with_value"));
      ExpectedM expected_error = ExpectedM(UnexpectedM(ErrorM("and_then_with_error")));

      auto expected_out = etl::move(expected).and_then([](ValueM v) -> ExpectedM {
        return ValueM(etl::move(v.v.append("_and_thened")));
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("and_then_with_value_and_thened", expected_out.value().v);

      auto with_value_type_check = check_expected_type_helper<ValueM, ErrorM>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = etl::move(expected_error).and_then([](ValueM&& v) -> ExpectedM {
        return ValueM(v.v.append("_and_thened"));
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<ValueM,ErrorM>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("and_then_with_error", unexpected_out.error().e);
    }

    //*************************************************************************
    
    TEST(test_and_then_void) {
      ExpectedV expected;
      ExpectedV expected_error = {Unexpected(Error("and_then_with_error"))};
      auto and_thened {false};

      auto expected_out = expected.and_then([&and_thened]() -> ExpectedV {
        and_thened = true;
        return ExpectedV();
      });

      CHECK_TRUE(and_thened);
      CHECK_TRUE(expected_out.has_value());

      auto with_value_type_check = check_expected_type_helper<void, Error>(expected_out);
      CHECK_TRUE(with_value_type_check);

      and_thened = false;
      auto unexpected_out = expected_error.and_then([&and_thened]() -> ExpectedV {
        and_thened = true;
        return ExpectedV();
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<void,Error>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("and_then_with_error", unexpected_out.error().e);
    }

    TEST(test_and_then_void_move) {
      ExpectedVM expected;
      ExpectedVM expected_error = {UnexpectedM(ErrorM("and_then_with_error"))};
      auto and_thened {false};

      auto expected_out = etl::move(expected).and_then([&and_thened]() -> ExpectedVM {
        and_thened = true;
        return ExpectedVM();
      });

      CHECK_TRUE(and_thened);
      CHECK_TRUE(expected_out.has_value());

      auto with_value_type_check = check_expected_type_helper<void, ErrorM>(expected_out);
      CHECK_TRUE(with_value_type_check);

      and_thened = false;
      auto unexpected_out = etl::move(expected_error).and_then([&and_thened]() -> ExpectedVM {
        and_thened = true;
        return ExpectedVM();
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<void,ErrorM>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("and_then_with_error", unexpected_out.error().e);
    }
    
    //*************************************************************************

    TEST(test_transform_error) {
      Expected expected = {Value("transform_error_with_value")};
      Expected expected_error = {Unexpected(Error("transform_error_with_error"))};

      auto expected_out = expected.transform_error([](Error e) {
        auto s = e.e.append("_transformed");
        return s;
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("transform_error_with_value", expected_out.value().v);

      auto with_value_type_check = check_expected_type_helper<Value, std::string>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = expected_error.transform_error([](Error e) {
        std::string s = e.e.append("_transformed");
        return s;
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<Value,std::string>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("transform_error_with_error_transformed", unexpected_out.error());
    }

    //*************************************************************************

    TEST(test_transform_error_move_constructor) {
      ExpectedM expected = ExpectedM(ValueM("transform_error_with_value"));
      ExpectedM expected_error = ExpectedM(UnexpectedM(ErrorM("transform_error_with_error")));

      auto expected_out = etl::move(expected).transform_error([](ErrorM e) {
        auto s = e.e.append("_transformed");
        return s;
      });

      CHECK_TRUE(expected_out.has_value());
      CHECK_EQUAL("transform_error_with_value", expected_out.value().v);

      auto with_value_type_check = check_expected_type_helper<ValueM, std::string>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = etl::move(expected_error).transform_error([](ErrorM e) {
        std::string s = e.e.append("_transformed");
        return s;
      });

      CHECK_FALSE(unexpected_out.has_value());

      auto with_error_type_check = check_expected_type_helper<ValueM,std::string>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

      CHECK_EQUAL("transform_error_with_error_transformed", unexpected_out.error());
    }

    TEST(test_transform_error_const_rvalue) {
      bool error_generated {false};
      auto temp_expected = Expected(Unexpected(Error("temp_const_error")));

      auto unexpected_out = static_cast<const Expected&&>(temp_expected)
          .transform_error([&error_generated](const Error& e) -> std::string {
              error_generated = true;
              CHECK_EQUAL("temp_const_error", e.e);
            
              return e.e; 
          });
      
      CHECK_TRUE(error_generated);
      CHECK_EQUAL("temp_const_error", unexpected_out.error());
    }

    TEST(test_transform_error_void_value) {
      ExpectedV expected;
      ExpectedV expected_error = UnexpectedV(Error("transform_error_void_value"));
      bool executed {false};

      auto expected_out = expected.transform_error([&executed](const Error& e) {
        executed = true;
        return e.e;
      });

      CHECK_FALSE(executed);
      CHECK_TRUE(expected_out.has_value());

      auto with_value_type_check = check_expected_type_helper<void, std::string>(expected_out);
      CHECK_TRUE(with_value_type_check);

      auto unexpected_out = expected_error.transform_error([&executed](const Error& e) {
        executed = true;
        auto s = e.e;
        return s.append("_transformed");
      });

      CHECK_TRUE(executed);
      CHECK_EQUAL("transform_error_void_value_transformed", unexpected_out.error());

      auto with_error_type_check = check_expected_type_helper<void, std::string>(unexpected_out);
      CHECK_TRUE(with_error_type_check);

    }
  };
}
