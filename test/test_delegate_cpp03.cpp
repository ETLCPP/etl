/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#define ETL_IN_DELEGATE_CPP03_UNIT_TEST

#include "etl/private/delegate_cpp03.h"
#include "etl/vector.h"

#include <vector>
#include <functional>
#include <algorithm>

namespace
{
  //*****************************************************************************
  enum class FunctionCalled : int
  {
    Not_Called,
    Free_Void_Called,
    Free_Int_Called,
    Free_Reference_Called,
    Free_Moveableonly_Called,
    Normal_Called,
    Normal_Returning_Void_Called,
    Alternative_Called,
    Member_Void_Called,
    Member_Void_Const_Called,
    Member_Int_Called,
    Member_Int_Const_Called,
    Member_Reference_Called,
    Member_Reference_Const_Called,
    Member_Moveableonly_Called,
    Member_Static_Called,
    Operator_Called,
    Operator_Const_Called,
    Lambda_Called
  };

  FunctionCalled function_called = FunctionCalled::Not_Called;

  //*****************************************************************************
  const int VALUE1 = 1;
  bool parameter_correct = false;

  //*****************************************************************************
  // Test data structure.
  //*****************************************************************************
  struct Data
  {
    int d;
  };

  //*****************************************************************************
  // The free function taking no parameters.
  //*****************************************************************************
  void free_void()
  {
    function_called = FunctionCalled::Free_Void_Called;
  }

  //*****************************************************************************
  // The free function taking an int parameter.
  //*****************************************************************************
  void free_int(int i)
  {
    function_called = FunctionCalled::Free_Int_Called;
    parameter_correct = (i == VALUE1);
  }

  //*****************************************************************************
  // The free function taking a Data reference parameter.
  //*****************************************************************************
  void free_reference(const Data& data)
  {
    function_called = FunctionCalled::Free_Reference_Called;
    parameter_correct = (data.d == VALUE1);
  }

  //*****************************************************************************
  // The normal function.
  //*****************************************************************************
  int normal(int i)
  {
    function_called = FunctionCalled::Normal_Called;
    parameter_correct = (i == VALUE1);

    return i;
  }

  //*****************************************************************************
  // The normal function returning void.
  //*****************************************************************************
  void normal_returning_void(int i)
  {
    function_called = FunctionCalled::Normal_Returning_Void_Called;
    parameter_correct = (i == VALUE1);
  }

  //*****************************************************************************
  // The alternative function.
  //*****************************************************************************
  int alternative(int i)
  {
    function_called = FunctionCalled::Alternative_Called;
    parameter_correct = (i == VALUE1);

    return i + 1;
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
      function_called = FunctionCalled::Member_Void_Called;
    }

    void member_void_const() const
    {
      function_called = FunctionCalled::Member_Void_Const_Called;
    }

    //*******************************************
    // int
    void member_int(int i)
    {
      function_called = FunctionCalled::Member_Int_Called;
      parameter_correct = (i == VALUE1);
    }

    void member_int_const(int i) const
    {
      function_called = FunctionCalled::Member_Int_Const_Called;
      parameter_correct = (i == VALUE1);
    }

    //*******************************************
    // reference
    void member_reference(const Data& data)
    {
      function_called = FunctionCalled::Member_Reference_Called;
      parameter_correct = (data.d == VALUE1);
    }

    void member_reference_const(const Data& data) const
    {
      function_called = FunctionCalled::Member_Reference_Const_Called;
      parameter_correct = (data.d == VALUE1);
    }

    //*******************************************
    // static
    static void member_static(const Data& data)
    {
      function_called = FunctionCalled::Member_Static_Called;
      parameter_correct = (data.d == VALUE1);
    }

    //*******************************************
    // operator()
    void operator()()
    {
      function_called = FunctionCalled::Operator_Called;
    }

    void operator()() const
    {
      function_called = FunctionCalled::Operator_Const_Called;
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
      function_called   = FunctionCalled::Not_Called;
      parameter_correct = false;
    }
  };

  SUITE(test_delegate)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_is_valid_false)
    {
      etl_cpp03::delegate<void(void)> d;

      CHECK(!d.is_valid());
      CHECK(!d);

      CHECK_THROW(d(), etl_cpp03::delegate_uninitialised);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_void)
    {
      auto d = etl_cpp03::delegate<void(void)>::create<free_void>();

      d();

      CHECK(function_called == FunctionCalled::Free_Void_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_int)
    {
      auto d = etl_cpp03::delegate<void(int)>::create<free_int>();

      d(VALUE1);

      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_reference)
    {
      auto d = etl_cpp03::delegate<void(const Data&)>::create<free_reference>();

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Free_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void)
    {
      Test test;

      etl_cpp03::delegate<void(void)> d(test);

      d();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_create)
    {
      Test test;

      auto d = etl_cpp03::delegate<void(void)>::create(test);

      d();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_const)
    {
      const Test test;

      etl_cpp03::delegate<void(void)> d(test);

      d();

      CHECK(function_called == FunctionCalled::Operator_Const_Called);
    }

#if !defined(ETL_COMPILER_GCC)
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time)
    {
      auto d = etl_cpp03::delegate<void(void)>::create<Test, test_static>();

      d();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time_const)
    {
      auto d = etl_cpp03::delegate<void(void)>::create<const Test, const_test_static>();

      d();

      CHECK(function_called == FunctionCalled::Operator_Const_Called);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_member_operator_void)
    {
      Test test;

      etl_cpp03::delegate<void(void)> d;

      d = test;

      d();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void)
    {
      Test test;

      auto d = etl_cpp03::delegate<void(void)>::create<Test, &Test::member_void>(test);

      d();

      CHECK(function_called == FunctionCalled::Member_Void_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_const)
    {
      const Test test;

      auto d = etl_cpp03::delegate<void(void)>::create<Test, &Test::member_void_const>(test);

      d();

      CHECK(function_called == FunctionCalled::Member_Void_Const_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int)
    {
      Test test;

      auto d = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test);

      d(VALUE1);

      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_const)
    {
      const Test test;

      auto d = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int_const>(test);

      d(VALUE1);

      CHECK(function_called == FunctionCalled::Member_Int_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference)
    {
      Test test;
      auto d = etl_cpp03::delegate<void(const Data&)>::create<Test, &Test::member_reference>(test);

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_const)
    {
      const Test test;
      auto d = etl_cpp03::delegate<void(const Data&)>::create<Test, &Test::member_reference_const>(test);

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_static)
    {
      auto d = etl_cpp03::delegate<void(const Data&)>::create<Test::member_static>();

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Static_Called);
      CHECK(parameter_correct);
    }

#if !(defined(ETL_COMPILER_GCC) && (__GNUC__ <= 5))
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_compile_time)
    {
      auto d = etl_cpp03::delegate<void(void)>::create<Test, test_static, &Test::member_void>();

      d();

      CHECK(function_called == FunctionCalled::Member_Void_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_const_compile_time)
    {
      auto d = etl_cpp03::delegate<void(void)>::create<Test, const_test_static, &Test::member_void_const>();

      d();

      CHECK(function_called == FunctionCalled::Member_Void_Const_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_compile_time)
    {
      auto d = etl_cpp03::delegate<void(int)>::create<Test, test_static, &Test::member_int>();

      d(VALUE1);

      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_const_compile_time)
    {
      auto d = etl_cpp03::delegate<void(int)>::create<Test, const_test_static, &Test::member_int_const>();

      d(VALUE1);

      CHECK(function_called == FunctionCalled::Member_Int_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_compile_time)
    {
      auto d = etl_cpp03::delegate<void(const Data&)>::create<Test, test_static, &Test::member_reference>();

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_const_compile_time)
    {
      auto d = etl_cpp03::delegate<void(const Data&)>::create<Test, const_test_static, &Test::member_reference_const>();

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_free_int)
    {
      etl_cpp03::delegate<void(int)> d;
      
      d.set<free_int>();

      d(VALUE1);

      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_lambda_int)
    {
      etl_cpp03::delegate<void(int)> d;
      
      d.set([](int i) { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1); });

      d(VALUE1);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference)
    {
      Test test;
      etl_cpp03::delegate<void(const Data&)> d;
      
      d.set<Test, &Test::member_reference>(test);

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_const_member_reference)
    {
      Test test;
      etl_cpp03::delegate<void(const Data&)> d;

      d.set<Test, &Test::member_reference_const>(test);

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_compile_time)
    {
      etl_cpp03::delegate<void(const Data&)> d;

      d.set<Test, test_static, &Test::member_reference>();

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_const_compile_time)
    {
      etl_cpp03::delegate<void(const Data&)> d;

      d.set<Test, const_test_static, &Test::member_reference_const>();

      Data data;
      data.d = VALUE1;

      d(data);

      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_construct)
    {
      Test test;

      auto d1 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test);
      auto d2(d1);

      d2(VALUE1);

      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment)
    {
      Test test;

      auto d1 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test);
      etl_cpp03::delegate<void(int)> d2;

      d2 = d1;

      d2(VALUE1);

      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_equal)
    {
      Test test;

      auto d1 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test);
      auto d2 = d1;

      CHECK(d1 == d2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_not_equal)
    {
      Test test;

      auto d1 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test);
      auto d2 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int_const>(test);;

      CHECK(d1 != d2);
    }

    //*************************************************************************
    TEST(test_issue_418)
    {
      etl::vector<etl_cpp03::delegate<int(int)>, 5> vector_of_delegates;

      vector_of_delegates.push_back(etl_cpp03::delegate<int(int)>::create<times_2>());

      CHECK_EQUAL(42, vector_of_delegates.front()(21));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_run_time_normal)
    {
      etl_cpp03::delegate<int(int)> d = etl_cpp03::delegate<int(int)>::create<normal>();

      int result = d.call_or(alternative, VALUE1);

      CHECK_EQUAL(VALUE1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_run_time_alternative)
    {
      etl_cpp03::delegate<int(int)> d;

      int result = d.call_or(alternative, VALUE1);

      CHECK_EQUAL(VALUE1 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_compile_time_alternative)
    {
      etl_cpp03::delegate<int(int)> d;

      int result = d.call_or<alternative>(VALUE1);

      CHECK_EQUAL(VALUE1 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_delegate_alternative)
    {
      etl_cpp03::delegate<int(int)> d;

      auto alt = etl_cpp03::delegate<int(int)>::create<alternative>();

      int result = d.call_or(alt, VALUE1);

      CHECK_EQUAL(VALUE1 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_valid)
    {
      auto d = etl_cpp03::delegate<int(int)>::create<normal>();

      etl::optional<int> result = d.call_if(VALUE1);

      CHECK(bool(result));
      CHECK_EQUAL(VALUE1, result.value());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_not_valid)
    {
      etl_cpp03::delegate<int(int)> d;

      etl::optional<int> result = d.call_if(VALUE1);

      CHECK(!bool(result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_valid_returning_void)
    {
      auto d = etl_cpp03::delegate<void(int)>::create<normal_returning_void>();

      bool was_called = d.call_if(VALUE1);

      CHECK(was_called);
      CHECK(function_called == FunctionCalled::Normal_Returning_Void_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_not_valid_returning_void)
    {
      etl_cpp03::delegate<void(int)> d;

      bool was_called = d.call_if(VALUE1);

      CHECK(!was_called);
      CHECK(function_called == FunctionCalled::Not_Called);
      CHECK(!parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_identification)
    {
      Test test1;
      Test test2;

      auto d1 = etl_cpp03::delegate<void(int)>::create<free_int>();
      auto d2 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test1);
      auto d3 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test2);

      etl_cpp03::delegate<void(int)> d4;

      using Delegate_List = std::vector<etl_cpp03::delegate<void(int)>>;

      Delegate_List delegate_list = { d1, d2, d3 };

      Delegate_List::const_iterator itr;

      d4 = d1;

      itr = std::find(delegate_list.begin(), delegate_list.end(), d4);
      CHECK(*itr == d1);
      CHECK(*itr != d2);
      CHECK(*itr != d3);

      d4 = d2;

      itr = std::find(delegate_list.begin(), delegate_list.end(), d4);
      CHECK(*itr != d1);
      CHECK(*itr == d2);
      CHECK(*itr != d3);

      d4 = d3;

      itr = std::find(delegate_list.begin(), delegate_list.end(), d4);
      CHECK(*itr != d1);
      CHECK(*itr != d2);
      CHECK(*itr == d3);

      d4 = etl_cpp03::delegate<void(int)>::create<Test, &Test::member_int>(test2); // Same as d3
      itr = std::find(delegate_list.begin(), delegate_list.end(), d4);
      CHECK(*itr != d1);
      CHECK(*itr != d2);
      CHECK(*itr == d3);
    }
  };
}
