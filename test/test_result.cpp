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

#include "etl/result.h"
#include "etl/type_traits.h"

#include <string>

namespace
{
  struct Value
  {
    std::string v;
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

    ErrorM(ErrorM&& other) = default;
    ErrorM& operator =(ErrorM&& rhs) = default;

    ErrorM(const ErrorM& other) = delete;
    ErrorM& operator =(const ErrorM& rhs) = delete;

    std::string e;
  };

  using ResultValueError = etl::result<Value,  Error>;
  using ResultVoidError  = etl::result<void,   Error>;
  using ResultValueVoid  = etl::result<Value,  void>;
  using ResultM          = etl::result<ValueM, ErrorM>;
}

namespace
{
  SUITE(test_result)
  {
    //*************************************************************************
    TEST(test_construct_result_value_error_type_with_value)
    {
      Value input = { "value 1" };
      ResultValueError result(input);

      Value output = result.value();

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(output.v == input.v);
    }

    //*************************************************************************
    TEST(test_construct_const_result_value_error_type_with_value)
    {
      Value input = { "value 1" };
      const ResultValueError result(input);

      const Value& output = result.value();

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(output.v == input.v);
    }

    //*************************************************************************
    TEST(test_construct_moveable_result_value_error_type_with_value)
    {
      ValueM input = { "value 1" };
      ResultM result(etl::move(input));

      ValueM output = etl::move(result.value());

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(output.v == std::string("value 1"));
    }

    //*************************************************************************
    TEST(test_construct_result_value_error_type_with_error)
    {
      Error input = { "error 1" };
      ResultValueError result(input);

      Error output = result.error();

      CHECK(!result.is_value());
      CHECK(result.is_error());
      CHECK(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_construct_const_result_value_error_type_with_error)
    {
      Error input = { "error 1" };
      const ResultValueError result(input);

      const Error& output = result.error();

      CHECK(!result.is_value());
      CHECK(result.is_error());
      CHECK(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_construct_moveable_result_value_error_type_with_error)
    {
      ErrorM input = { "error 1" };
      ResultM result(etl::move(input));

      ErrorM output = etl::move(result.error());

      CHECK(!result.is_value());
      CHECK(result.is_error());
      CHECK(output.e == std::string("error 1"));
    }

    //*************************************************************************
    TEST(test_default_construct_result_void_error_type)
    {
       ResultVoidError result;

       CHECK(result.is_value());
       CHECK(!result.is_error());
    }

    //*************************************************************************
    TEST(test_default_construct_const_result_void_error_type)
    {
       const ResultVoidError result;

       CHECK(result.is_value());
       CHECK(!result.is_error());
    }

    //*************************************************************************
    TEST(test_construct_result_void_error_type_with_error)
    {
      Error input = { "error 1" };
      ResultVoidError result(input);

      Error output = result.error();

      CHECK(!result.is_value());
      CHECK(result.is_error());
      CHECK(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_construct_const_result_void_error_type_with_error)
    {
      Error input = { "error 1" };
      const ResultVoidError result(input);

      const Error& output = result.error();

      CHECK(!result.is_value());
      CHECK(result.is_error());
      CHECK(output.e == input.e);
    }

    //*************************************************************************
    TEST(test_construct_moveable_result_void_error_type_with_error)
    {
      Error input = { "error 1" };
      ResultVoidError result(etl::move(input));

      Error output = etl::move(result.error());

      CHECK(!result.is_value());
      CHECK(result.is_error());
      CHECK(output.e == std::string("error 1"));
    }

    //*************************************************************************
    TEST(test_construct_moveable_const_result_void_error_type_with_error)
    {
      Error input = { "error 1" };
      const ResultVoidError result(etl::move(input));

      Error output = etl::move(result.error());

      CHECK(!result.is_value());
      CHECK(result.is_error());
      CHECK(output.e == std::string("error 1"));
    }

    //*************************************************************************
    TEST(test_default_construct_result_value_void_type)
    {
      ResultValueVoid result;

      CHECK(!result.is_value());
      CHECK(result.is_error());
    }

    //*************************************************************************
    TEST(test_default_construct_const_result_value_void_type)
    {
      const ResultValueVoid result;

      CHECK(!result.is_value());
      CHECK(result.is_error());
    }

    //*************************************************************************
    TEST(test_construct_result_value_void_type_with_value)
    {
      Value input = { "value 1" };
      ResultValueVoid result(input);

      Value output = result.value();

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(output.v == input.v);
    }

    //*************************************************************************
    TEST(test_construct_const_result_value_void_type_with_value)
    {
      Value input = { "value 1" };
      const ResultValueVoid result(input);

      const Value output = result.value();

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(output.v == input.v);
    }

    //*************************************************************************
    TEST(test_construct_moveable_result_value_void_type_with_value)
    {
      Value input = { "value 1" };
      ResultValueVoid result(etl::move(input));

      Value output = etl::move(result.value());

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(output.v == std::string("value 1"));
    }

    //*************************************************************************
    TEST(test_construct_moveable_const_result_value_void_type_with_value)
    {
      Value input = { "value 1" };
      const ResultValueVoid result(etl::move(input));

      Value output = etl::move(result.value());

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(output.v == std::string("value 1"));
    }

    //*************************************************************************
    TEST(test_copy_construct_result)
    {
      Value input = { "value 1" };
      ResultValueError result(input);

      ResultValueError result2(result);

      Value output = result2.value();

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(result2.is_value());
      CHECK(!result2.is_error());
      CHECK(output.v == input.v);
    }

    //*************************************************************************
    TEST(test_move_construct_result)
    {
      Value input = { "value 1" };
      ResultValueError result(input);

      ResultValueError result2(etl::move(result));

      Value output = result.value();
      Value output2 = result2.value();

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(result2.is_value());
      CHECK(!result2.is_error());
      CHECK(output.v != input.v);
      CHECK(output2.v == input.v);
    }

    //*************************************************************************
    TEST(test_move_assign_result)
    {
      Value input = { "value 1" };
      ResultValueError result(input);

      Value input2 = { "value 2" };
      ResultValueError result2(result);

      result2 = etl::move(result);

      Value output  = result.value();
      Value output2 = result2.value();

      CHECK(result.is_value());
      CHECK(!result.is_error());
      CHECK(result2.is_value());
      CHECK(!result2.is_error());
      CHECK(output.v != input.v);
      CHECK(output2.v == input.v);
    }
  };
}
