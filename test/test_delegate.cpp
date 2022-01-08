/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#include "etl/private/delegate_cpp11.h"
#include "etl/vector.h"

#if !defined(ETL_CRC_FORCE_CPP03_IMPLEMENTATION)

#include <functional>

namespace
{
  //*****************************************************************************
  const int VALUE1 = 1;
  const int VALUE2 = 2;
  bool function_called = false;
  bool parameter_correct = false;

  //*****************************************************************************
  // Test data structure.
  //*****************************************************************************
  struct Data
  {
    int d;
  };

  //*****************************************************************************
  // Test moveable only data structure.
  //*****************************************************************************
  struct MoveableOnlyData
  {
    MoveableOnlyData() = default;
    ~MoveableOnlyData() = default;
    MoveableOnlyData(const MoveableOnlyData&) = delete;
    MoveableOnlyData& operator=(const MoveableOnlyData&) = delete;
    MoveableOnlyData(MoveableOnlyData&&) = default;
    MoveableOnlyData& operator=(MoveableOnlyData&&) = default;
    int d;
  };

  //*****************************************************************************
  // The free function taking no parameters.
  //*****************************************************************************
  void free_void()
  {
    function_called = true;
  }

  //*****************************************************************************
  // The free function taking an int parameter.
  //*****************************************************************************
  void free_int(int i, int j)
  {
    function_called = true;
    parameter_correct = (i == VALUE1) && (j == VALUE2);
  }

  //*****************************************************************************
  // The free function taking a Data reference parameter.
  //*****************************************************************************
  void free_reference(const Data& data, int j)
  {
    function_called = true;
    parameter_correct = (data.d == VALUE1) && (j == VALUE2);
  }

  //*****************************************************************************
  // The free function taking a moveable only parameter.
  //*****************************************************************************
  void free_moveableonly(MoveableOnlyData&& data)
  {
    function_called = true;
    parameter_correct = (data.d == VALUE1);
  }

  //*****************************************************************************
  // The normal function.
  //*****************************************************************************
  int normal(int i, int j)
  {
    function_called = true;
    parameter_correct = (i == VALUE1) && (j == VALUE2);

    return i + j;
  }

  //*****************************************************************************
  // The normal function returning void.
  //*****************************************************************************
  void normal_returning_void(int i, int j)
  {
    function_called = true;
    parameter_correct = (i == VALUE1) && (j == VALUE2);
  }

  //*****************************************************************************
  // The alternative function.
  //*****************************************************************************
  int alternative(int i, int j)
  {
    function_called = true;
    parameter_correct = (i == VALUE1) && (j == VALUE2);

    return i + j + 1;
  }

  //*****************************************************************************
  // The test class with member functions.
  //*****************************************************************************
  class Test
  {
  public:

    //*******************************************
    // void
    void member_void()
    {
      function_called = true;
    }

    void member_void_const() const
    {
      function_called = true;
    }

    //*******************************************
    // int
    void member_int(int i, int j)
    {
      function_called = true;
      parameter_correct = (i == VALUE1) && (j == VALUE2);
    }

    void member_int_const(int i, int j) const
    {
      function_called = true;
      parameter_correct = (i == VALUE1) && (j == VALUE2);
    }

    //*******************************************
    // reference
    void member_reference(const Data& data, int j)
    {
      function_called = true;
      parameter_correct = (data.d == VALUE1) && (j == VALUE2);
    }

    void member_reference_const(const Data& data, int j) const
    {
      function_called = true;
      parameter_correct = (data.d == VALUE1) && (j == VALUE2);
    }

    //*******************************************
    // moveable only data
    void member_moveableonly(MoveableOnlyData&& data)
    {
      function_called = true;
      parameter_correct = (data.d == VALUE1);
    }

    //*******************************************
    // static
    static void member_static(const Data& data, int j)
    {
      function_called = true;
      parameter_correct = (data.d == VALUE1) && (j == VALUE2);
    }

    //*******************************************
    // operator()
    void operator()()
    {
      function_called = true;
    }

    void operator()() const
    {
      function_called = true;
    }
  };

  //*******************************************
  int times_2(int a) 
  {
    return a * 2;
  }

  Test test_static;
  const Test const_test_static;
}

namespace
{
  //*****************************************************************************
  // Initialises the test results.
  //*****************************************************************************
  struct SetupFixture
  {
    SetupFixture()
    {
      function_called   = false;
      parameter_correct = false;
    }
  };

  SUITE(test_delegate)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_is_valid_false)
    {
      etl::delegate<void(void)> d;

      CHECK(!d.is_valid());
      CHECK(!d);

      CHECK_THROW(d(), etl::delegate_uninitialised);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constexpr_is_valid_false)
    {
      constexpr etl::delegate<void(void)> d;

      CHECK(!d.is_valid());
      CHECK(!d);

      CHECK_THROW(d(), etl::delegate_uninitialised);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_is_valid_true)
    {
      etl::delegate<void(void)> d([] {});

      CHECK(d.is_valid());
      CHECK(d);
      CHECK_NO_THROW(d());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_void)
    {
      auto d = etl::delegate<void(void)>::create<free_void>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_void_constexpr)
    {
      constexpr auto d = etl::delegate<void(void)>::create<free_void>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_int)
    {
      auto d = etl::delegate<void(int, int)>::create<free_int>();

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_int_constexpr)
    {
      constexpr auto d = etl::delegate<void(int, int)>::create<free_int>();

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_reference)
    {
      etl::delegate<void(const Data&, int)> d = etl::delegate<void(const Data&, int)>::create<free_reference>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_reference_constexpr)
    {
      constexpr etl::delegate<void(const Data&, int)> d = etl::delegate<void(const Data&, int)>::create<free_reference>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_moveableonly)
    {
      auto d = etl::delegate<void(MoveableOnlyData&&)>::create<free_moveableonly>();

      MoveableOnlyData data;
      data.d = VALUE1;

      d(std::move(data));

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_moveableonly_constexpr)
    {
      constexpr auto d = etl::delegate<void(MoveableOnlyData&&)>::create<free_moveableonly>();

      MoveableOnlyData data;
      data.d = VALUE1;

      d(std::move(data));

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lambda_int)
    {
      etl::delegate<void(int, int)> d([](int i, int j) { function_called = true; parameter_correct = (i == VALUE1) && (j == VALUE2); });

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lambda_int_create)
    {
      auto lambda = [](int i, int j) { function_called = true; parameter_correct = (i == VALUE1) && (j == VALUE2); };

      etl::delegate<void(int, int)> d(lambda);

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void)
    {
      Test test;

      etl::delegate<void(void)> d(test);

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_create)
    {
      Test test;

      auto d = etl::delegate<void(void)>::create(test);

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_create_constexpr)
    {
      static Test test;

      constexpr auto d = etl::delegate<void(void)>::create(test);

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_const)
    {
      const Test test;

      etl::delegate<void(void)> d(test);

      d();

      CHECK(function_called);
    }

#if !defined(ETL_COMPILER_GCC)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time)
    {
      auto d = etl::delegate<void(void)>::create<Test, test_static>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time_constexpr)
    {
      constexpr auto d = etl::delegate<void(void)>::create<Test, test_static>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time_const)
    {
      auto d = etl::delegate<void(void)>::create<const Test, const_test_static>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time_const_constexpr)
    {
      constexpr auto d = etl::delegate<void(void)>::create<const Test, const_test_static>();

      d();

      CHECK(function_called);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_member_operator_void)
    {
      Test test;

      etl::delegate<void(void)> d;

      d = test;

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void)
    {
      Test test;

      auto d = etl::delegate<void(void)>::create<Test, &Test::member_void>(test);

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_constexpr)
    {
      static Test test;

      constexpr auto d = etl::delegate<void(void)>::create<Test, &Test::member_void>(test);

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_const)
    {
      const Test test;

      auto d = etl::delegate<void(void)>::create<Test, &Test::member_void_const>(test);

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_const_constexpr)
    {
      static const Test test;

      constexpr auto d = etl::delegate<void(void)>::create<Test, &Test::member_void_const>(test);

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int)
    {
      Test test;

      auto d = etl::delegate<void(int, int)>::create<Test, &Test::member_int>(test);

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_constexpr)
    {
      static Test test;

      constexpr auto d = etl::delegate<void(int, int)>::create<Test, &Test::member_int>(test);

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_const)
    {
      const Test test;

      auto d = etl::delegate<void(int, int)>::create<Test, &Test::member_int_const>(test);

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_const_constexpr)
    {
      static const Test test;

      constexpr auto d = etl::delegate<void(int, int)>::create<Test, &Test::member_int_const>(test);

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference)
    {
      Test test;
      auto d = etl::delegate<void(const Data&, int)>::create<Test, &Test::member_reference>(test);

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_constexpr)
    {
      static Test test;
      constexpr auto d = etl::delegate<void(const Data&, int)>::create<Test, &Test::member_reference>(test);

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_const)
    {
      const Test test;
      auto d = etl::delegate<void(const Data&, int)>::create<Test, &Test::member_reference_const>(test);

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_const_constexpr)
    {
      static const Test test;
      constexpr auto d = etl::delegate<void(const Data&, int)>::create<Test, &Test::member_reference_const>(test);

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_moveableonly)
    {
      Test test;
      auto d = etl::delegate<void(MoveableOnlyData&&)>::create<Test, &Test::member_moveableonly>(test);

      MoveableOnlyData data;
      data.d = VALUE1;

      d(std::move(data));

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_moveableonly_constexpr)
    {
      static Test test;
      constexpr auto d = etl::delegate<void(MoveableOnlyData&&)>::create<Test, &Test::member_moveableonly>(test);

      MoveableOnlyData data;
      data.d = VALUE1;

      d(std::move(data));

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_static)
    {
      auto d = etl::delegate<void(const Data&, int)>::create<Test::member_static>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_static_constexpr)
    {
      constexpr auto d = etl::delegate<void(const Data&, int)>::create<Test::member_static>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 5))
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_compile_time)
    {
      auto d = etl::delegate<void(void)>::create<Test, test_static, &Test::member_void>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_compile_time_constexpr)
    {
      constexpr auto d = etl::delegate<void(void)>::create<Test, test_static, &Test::member_void>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_const_compile_time)
    {
      auto d = etl::delegate<void(void)>::create<Test, const_test_static, &Test::member_void_const>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_const_compile_time_constexpr)
    {
      constexpr auto d = etl::delegate<void(void)>::create<Test, const_test_static, &Test::member_void_const>();

      d();

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_compile_time)
    {
      auto d = etl::delegate<void(int, int)>::create<Test, test_static, &Test::member_int>();

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_compile_time_constexpr)
    {
      constexpr auto d = etl::delegate<void(int, int)>::create<Test, test_static, &Test::member_int>();

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_const_compile_time)
    {
      auto d = etl::delegate<void(int, int)>::create<Test, const_test_static, &Test::member_int_const>();

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_const_compile_time_constexpr)
    {
      constexpr auto d = etl::delegate<void(int, int)>::create<Test, const_test_static, &Test::member_int_const>();

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_compile_time)
    {
      auto d = etl::delegate<void(const Data&, int)>::create<Test, test_static, &Test::member_reference>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_compile_time_constexpr)
    {
      constexpr auto d = etl::delegate<void(const Data&, int)>::create<Test, test_static, &Test::member_reference>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_const_compile_time)
    {
      auto d = etl::delegate<void(const Data&, int)>::create<Test, const_test_static, &Test::member_reference_const>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_const_compile_time_constexpr)
    {
      constexpr auto d = etl::delegate<void(const Data&, int)>::create<Test, const_test_static, &Test::member_reference_const>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_free_int)
    {
      etl::delegate<void(int, int)> d;
      
      d.set<free_int>();

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_lambda_int)
    {
      etl::delegate<void(int, int)> d;
      
      d.set([](int i, int j) { function_called = true; parameter_correct = (i == VALUE1) && (j == VALUE2); });

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference)
    {
      Test test;
      etl::delegate<void(const Data&, int)> d;
      
      d.set<Test, &Test::member_reference>(test);

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_const_member_reference)
    {
      Test test;
      etl::delegate<void(const Data&, int)> d;

      d.set<Test, &Test::member_reference_const>(test);

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_compile_time)
    {
      etl::delegate<void(const Data&, int)> d;

      d.set<Test, test_static, &Test::member_reference>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_const_compile_time)
    {
      etl::delegate<void(const Data&, int)> d;

      d.set<Test, const_test_static, &Test::member_reference_const>();

      Data data;
      data.d = VALUE1;

      d(data, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_construct)
    {
      Test test;

      auto d1 = etl::delegate<void(int, int)>::create<Test, &Test::member_int>(test);
      auto d2(d1);

      d2(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_construct_constexpr)
    {
      static Test test;

      constexpr auto d1 = etl::delegate<void(int, int)>::create<Test, &Test::member_int>(test);
      constexpr auto d2(d1);

      d2(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Test test;

      auto d1 = etl::delegate<void(int, int)>::create<Test, &Test::member_int>(test);
      etl::delegate<void(int, int)> d2;

      d2 = d1;

      d2(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_equal)
    {
      Test test;

      auto d1 = etl::delegate<void(int, int)>::create<Test, &Test::member_int>(test);
      auto d2 = d1;

      CHECK(d1 == d2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_not_equal)
    {
      Test test;

      auto d1 = etl::delegate<void(int, int)>::create<Test, &Test::member_int>(test);
      auto d2 = etl::delegate<void(int, int)>::create<Test, &Test::member_int_const>(test);;

      CHECK(d1 != d2);
    }

    //*************************************************************************
    TEST(test_issue_418)
    {
      etl::vector<etl::delegate<int(int)>, 5> vector_of_delegates;

      vector_of_delegates.push_back(etl::delegate<int(int)>::create<times_2>());

      CHECK_EQUAL(42, vector_of_delegates.front()(21));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_run_time_normal)
    {
      auto d = etl::delegate<int(int, int)>::create<normal>();

      int result = d.call_or(alternative, VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_run_time_alternative)
    {
      etl::delegate<int(int, int)> d;

      int result = d.call_or(alternative, VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_compile_time_alternative)
    {
      etl::delegate<int(int, int)> d;

      int result = d.call_or<alternative>(VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_delegate_alternative)
    {
      etl::delegate<int(int, int)> d;

      auto alt = etl::delegate<int(int, int)>::create<alternative>();

      int result = d.call_or(alt, VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_valid)
    {
      auto d = etl::delegate<int(int, int)>::create<normal>();

      etl::optional<int> result = d.call_if(VALUE1, VALUE2);

      CHECK(bool(result));
      CHECK_EQUAL(VALUE1 + VALUE2, result.value());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_not_valid)
    {
      etl::delegate<int(int, int)> d;

      etl::optional<int> result = d.call_if(VALUE1, VALUE2);

      CHECK(!bool(result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_valid_returning_void)
    {
      auto d = etl::delegate<void(int, int)>::create<normal_returning_void>();

      bool was_called = d.call_if(VALUE1, VALUE2);

      CHECK(was_called);
      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_not_valid_returning_void)
    {
      etl::delegate<void(int, int)> d;

      bool was_called = d.call_if(VALUE1, VALUE2);

      CHECK(!was_called);
      CHECK(!function_called);
      CHECK(!parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_construct_from_std_function_from_free_int)
    {
      std::function<void(int, int)> std_function(free_int);

      etl::delegate<void(int, int)> d(std_function);

      d(VALUE1, VALUE2);

      CHECK(function_called);
      CHECK(parameter_correct);
    }
  };
}

#endif
