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

#include "etl/inplace_function.h"
#include "etl/vector.h"

#include <vector>
#include <functional>
#include <algorithm>
#include <type_traits>
#include <stdexcept>

// Enable exactly one of these at a time to see the corresponding static_assert fire.
//#define ETL_NEGATIVE_TEST_INPLACE_FUNCTION_BAD_RETURN
//#define ETL_NEGATIVE_TEST_INPLACE_FUNCTION_RVALUE_PARAM_MISMATCH_NONCONST
//#define ETL_NEGATIVE_TEST_INPLACE_FUNCTION_RVALUE_PARAM_MISMATCH_CONST

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
  const int VALUE2 = 2;
  bool parameter_correct = false;

  //*****************************************************************************
  // Object data structure.
  //*****************************************************************************
  struct Data
  {
    int ipf;
  };

  //*****************************************************************************
  // Object moveable only data structure.
  //*****************************************************************************
  struct MoveableOnlyData
  {
    MoveableOnlyData() = default;
    ~MoveableOnlyData() = default;
    MoveableOnlyData(const MoveableOnlyData&) = delete;
    MoveableOnlyData& operator=(const MoveableOnlyData&) = delete;
    MoveableOnlyData(MoveableOnlyData&&) = default;
    MoveableOnlyData& operator=(MoveableOnlyData&&) = default;
    int ipf;
  };

  //*****************************************************************************
  // The free function taking no parameters.
  //*****************************************************************************
  static void free_void()
  {
    function_called = FunctionCalled::Free_Void_Called;
  }

  //*****************************************************************************
  // The free function taking an int parameter.
  //*****************************************************************************
  void free_int(int i, int j)
  {
    function_called = FunctionCalled::Free_Int_Called;
    parameter_correct = (i == VALUE1) && (j == VALUE2);
  }

  //*****************************************************************************
  // The free function taking a Data reference parameter.
  //*****************************************************************************
  void free_reference(const Data& data, int j)
  {
    function_called = FunctionCalled::Free_Reference_Called;
    parameter_correct = (data.ipf == VALUE1) && (j == VALUE2);
  }

  //*****************************************************************************
  // The free function taking a moveable only parameter.
  //*****************************************************************************
  void free_moveableonly(MoveableOnlyData&& data)
  {
    function_called = FunctionCalled::Free_Moveableonly_Called;
    parameter_correct = (data.ipf == VALUE1);
  }

  //*****************************************************************************
  // The normal function.
  //*****************************************************************************
  int normal(int i, int j)
  {
    function_called = FunctionCalled::Normal_Called;
    parameter_correct = (i == VALUE1) && (j == VALUE2);

    return i + j;
  }

  //*****************************************************************************
  // The normal function returning void.
  //*****************************************************************************
  void normal_returning_void(int i, int j)
  {
    function_called = FunctionCalled::Normal_Returning_Void_Called;
    parameter_correct = (i == VALUE1) && (j == VALUE2);
  }

  //*****************************************************************************
  // The alternative function.
  //*****************************************************************************
  int alternative(int i, int j)
  {
    function_called = FunctionCalled::Alternative_Called;
    parameter_correct = (i == VALUE1) && (j == VALUE2);

    return i + j + 1;
  }

  //*****************************************************************************
  // The throwing function.
  //*****************************************************************************
  void throwing_void()
  {
    throw std::runtime_error("throwing function");
  }

  int throwing_normal(int, int)
  {
    throw std::runtime_error("throwing function with two parameters");
  }

  //*******************************************
  // Functor with non-const operator()
  struct Functor
  {
    void operator()()
    {
      function_called = FunctionCalled::Operator_Called;
    }
  };

  //*******************************************
  // Functor with const operator()
  struct FunctorConst
  {
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

  //*******************************************
  // Free int return.
  int free_int_return(int i, int j)
  {
    return i + j;
  }

#if ETL_USING_CPP17
  static auto global_lambda = [](int i, int j)
    {
      function_called = FunctionCalled::Lambda_Called;
      parameter_correct = (i == VALUE1) && (j == VALUE2);
    };
#endif

  //*******************************************
  // Functor with that is destructible and movable
  //*******************************************
  struct DestructibleMovableObject
  {
    DestructibleMovableObject()
      : call_count(0)
    {
      destructor_called        = false;
      copy_constructor_called  = false;
      move_constructor_called  = false;
      function_operator_called = false;
    }

    ~DestructibleMovableObject()
    {
      destructor_called = true;
    }

    DestructibleMovableObject(const DestructibleMovableObject& other)
      : call_count(other.call_count)
    {
      copy_constructor_called = true;
    }

    DestructibleMovableObject(DestructibleMovableObject&& other)
      : call_count(other.call_count)
    {
      move_constructor_called = true;
    }

    void operator()(int, int)
    {
      function_operator_called = true;
      ++call_count;
    }

    static bool destructor_called;
    static bool copy_constructor_called;
    static bool move_constructor_called;
    static bool function_operator_called;

    int call_count;
  };

  bool DestructibleMovableObject::destructor_called        = false;
  bool DestructibleMovableObject::copy_constructor_called  = false;
  bool DestructibleMovableObject::move_constructor_called  = false;
  bool DestructibleMovableObject::function_operator_called = false;

  //*******************************************
  // Sized functor
  template <size_t Size>
  struct FunctorSized
  {
    FunctorSized()
      : data()
    {
    }

    int operator()()
    {
      function_called = FunctionCalled::Operator_Called;

      return Size;
    }

    int size() const
    {
      return Size;
    }

    char data[Size];
  };
}

//*****************************************************************************
// The test class with member functions.
//*****************************************************************************
class Object
{
public:

  //*******************************************
  constexpr Object()
    : call_count(0)
  {
  }

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
  int member_int(int i, int j)
  {
    function_called = FunctionCalled::Member_Int_Called;
    parameter_correct = (i == VALUE1) && (j == VALUE2);

    ++call_count;

    return i + j + 1; 
  }

  int member_int_const(int i, int j) const
  {
    function_called = FunctionCalled::Member_Int_Const_Called;
    parameter_correct = (i == VALUE1) && (j == VALUE2);

    ++call_count;

    return i + j + 1;
  }

  //*******************************************
  // reference
  void member_reference(const Data& data, int j)
  {
    function_called = FunctionCalled::Member_Reference_Called;
    parameter_correct = (data.ipf == VALUE1) && (j == VALUE2);
  }

  void member_reference_const(const Data& data, int j) const
  {
    function_called = FunctionCalled::Member_Reference_Const_Called;
    parameter_correct = (data.ipf == VALUE1) && (j == VALUE2);

    ++call_count;
  }

  //*******************************************
  // moveable only data
  void member_moveableonly(MoveableOnlyData&& data)
  {
    function_called = FunctionCalled::Member_Moveableonly_Called;
    parameter_correct = (data.ipf == VALUE1);

    ++call_count;
  }

  //*******************************************
  // static
  static void member_static(const Data& data, int j)
  {
    function_called = FunctionCalled::Member_Static_Called;
    parameter_correct = (data.ipf == VALUE1) && (j == VALUE2);
  }

  //*******************************************
  // operator()
  void operator()()
  {
    function_called = FunctionCalled::Operator_Called;

    ++call_count;
  }

  //*******************************************
  // operator() const
  void operator()() const
  {
    function_called = FunctionCalled::Operator_Const_Called;

    ++call_count;
  }

  mutable int call_count;
};

Object object_static;
constexpr const Object const_object_static;

Functor functor_static;
const FunctorConst const_functor_static;

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

  SUITE(test_inplace_function)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_inplace_function_types)
    {
      using InplaceFunction = etl::inplace_function<int(float, long)>;

      // Check the return type.
      CHECK_TRUE((std::is_same<InplaceFunction::return_type, int>::value));

      // Check the argument types.
      CHECK_TRUE((std::is_same<InplaceFunction::argument_types, etl::type_list<float, long>>::value));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_inplace_function_size_and_alignment)
    {
      using InplaceFunction84  = etl::inplace_function<int(float, long),  8, 4>;
      using InplaceFunction168 = etl::inplace_function<int(float, long), 16, 8>;

      // Check the sizes.
      CHECK_EQUAL(8,  InplaceFunction84::size());
      CHECK_EQUAL(16, InplaceFunction168::size());

      // Check the alignments.
      CHECK_EQUAL(4, InplaceFunction84::alignment());
      CHECK_EQUAL(8, InplaceFunction168::alignment());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_is_valid_false)
    {
      etl::inplace_function<void(void)> ipf;

      CHECK(!ipf.is_valid());
      CHECK(!ipf);

      CHECK_THROW(ipf(), etl::inplace_function_uninitialized);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_is_valid_true)
    {
      auto lambda = [] {};

      etl::inplace_function<void(void)> ipf(lambda);

      CHECK(ipf.is_valid());
      CHECK(ipf);
      CHECK_NO_THROW(ipf());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_is_valid_after_clear)
    {
      auto lambda = [] {};

      etl::inplace_function<void(void)> ipf(lambda);

      CHECK_TRUE(ipf.is_valid());
      ipf.clear();
      CHECK_FALSE(ipf.is_valid());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_void_run_time)
    {
      etl::inplace_function<void(void)> ipf(free_void);

      ipf();

      CHECK(function_called == FunctionCalled::Free_Void_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_void_compile_time)
    {
      auto ipf = etl::inplace_function<void(void)>::create<free_void>();

      ipf();

      CHECK(function_called == FunctionCalled::Free_Void_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_void_run_time)
    {
      auto ipf = etl::make_inplace_function(free_void);

      ipf();

      CHECK(function_called == FunctionCalled::Free_Void_Called);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_void_compile_time)
    {
      auto ipf = etl::make_inplace_function<&free_void>();

      ipf();

      CHECK(function_called == FunctionCalled::Free_Void_Called);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_throwing)
    {
      {
        auto ipf = etl::inplace_function<void(void)>::create<throwing_void>();

        CHECK_THROW(ipf(),         std::runtime_error);
        CHECK_THROW(ipf.call_if(), std::runtime_error);
      }

      {
        auto ipf = etl::inplace_function<int(int, int)>::create<throwing_normal>();

        CHECK_THROW({ipf.call_or(alternative, VALUE1, VALUE2);}, std::runtime_error);
        CHECK_THROW({ipf.call_or<alternative>(VALUE1, VALUE2);}, std::runtime_error);
      }
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_int_run_time)
    {
      etl::inplace_function<void(int, int)> ipf(free_int);

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }
    
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_int_compile_time)
    {
      auto ipf = etl::inplace_function<void(int, int)>::create<free_int>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_int_run_time)
    {
      auto ipf = etl::make_inplace_function(free_int);

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_int_compile_time)
    {
      auto ipf = etl::make_inplace_function<&free_int>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_reference_run_time)
    {
      etl::inplace_function<void(const Data&, int)> ipf(free_reference);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_reference_compile_time)
    {
      auto ipf = etl::inplace_function<void(const Data&, int)>::create<free_reference>();

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_reference_run_time)
    {
      auto ipf = etl::make_inplace_function(free_reference);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Reference_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_reference_compile_time)
    {
      auto ipf = etl::make_inplace_function<&free_reference>();

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Reference_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_moveableonly_run_time)
    {
      etl::inplace_function<void(MoveableOnlyData&&)> ipf(free_moveableonly);

      MoveableOnlyData data;
      data.ipf = VALUE1;

      ipf(std::move(data));

      CHECK(function_called == FunctionCalled::Free_Moveableonly_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_moveableonly_compile_time)
    {
      auto ipf = etl::inplace_function<void(MoveableOnlyData&&)>::create<free_moveableonly>();

      MoveableOnlyData data;
      data.ipf = VALUE1;

      ipf(std::move(data));

      CHECK(function_called == FunctionCalled::Free_Moveableonly_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_moveableonly_run_time)
    {
      auto ipf = etl::make_inplace_function(free_moveableonly);

      MoveableOnlyData data;
      data.ipf = VALUE1;

      ipf(std::move(data));

      CHECK(function_called == FunctionCalled::Free_Moveableonly_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_free_moveableonly_compile_time)
    {
      auto ipf = etl::make_inplace_function<&free_moveableonly>();

      MoveableOnlyData data;
      data.ipf = VALUE1;

      ipf(std::move(data));

      CHECK(function_called == FunctionCalled::Free_Moveableonly_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_lambda_int)
    {
      auto lambda = [](int i, int j) { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1) && (j == VALUE2); };

      etl::inplace_function<void(int, int)> ipf(lambda);

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_lambda_int_run_time)
    {
      auto lambda = [](int i, int j) { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1) && (j == VALUE2); };

      auto ipf = etl::make_inplace_function(lambda);

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_lambda_int_compile_time)
    {
      static auto lambda = [](int i, int j) { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1) && (j == VALUE2); };

      auto ipf = etl::make_inplace_function<decltype(lambda), lambda>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_capturing_lambda_int)
    {
      int value = VALUE1;

      auto lambda = [value](int i, int j) -> int { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1) && (j == VALUE2); return value + VALUE1 + VALUE2; };

      etl::inplace_function<int(int, int)> ipf(lambda);

      int result = ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
      CHECK_EQUAL(result, VALUE1 + VALUE1 + VALUE2);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_capturing_lambda_int_run_time)
    {
      int value = VALUE1;

      auto lambda = [value](int i, int j) -> int { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1) && (j == VALUE2); return value + VALUE1 + VALUE2; };

      auto ipf = etl::make_inplace_function(lambda);

      int result = ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
      CHECK_EQUAL(result, VALUE1 + VALUE1 + VALUE2);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_capturing_lambda_int_compile_time)
    {
      int value = VALUE1;

      static auto lambda = [value](int i, int j) -> int { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1) && (j == VALUE2); return value + VALUE1 + VALUE2; };

      auto ipf = etl::make_inplace_function<decltype(lambda), lambda>();

      int result = ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
      CHECK_EQUAL(result, VALUE1 + VALUE1 + VALUE2);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void)
    {
      Object object;

      etl::inplace_function<void(void)> ipf(object);

      ipf();

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_const)
    {
      const Object object;

      etl::inplace_function<void(void)> ipf(object);

      ipf();

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Operator_Const_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_operator_void_const)
    {
      const FunctorConst object;

      auto ipf = etl::make_inplace_function(object);

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Const_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time)
    {
      auto ipf = etl::inplace_function<void(void)>::create<Object, object_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_operator_void_compile_time_const)
    {
      auto ipf = etl::inplace_function<void(void)>::create<const Object, const_object_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Const_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_functor_operator_void_run_time)
    {
      auto ipf = etl::make_inplace_function(functor_static);

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST_FIXTURE(SetupFixture, test_make_inplace_functor_operator_void_compile_time)
    {
      auto ipf = etl::make_inplace_function<Functor, functor_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_operator_const_void_run_time)
    {
      auto ipf = etl::make_inplace_function(const_functor_static);

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Const_Called);
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_operator_const_void_compile_time)
    {
      auto ipf = etl::make_inplace_function<const FunctorConst, const_functor_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Const_Called);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_member_operator_void)
    {
      Object object;

      etl::inplace_function<void(void)> ipf;

      ipf = object;

      ipf();

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void)
    {
      auto ipf = etl::inplace_function<void(void)>::create<Object, &Object::member_void, object_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Member_Void_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_void_run_time)
    {
      static Object object;

      auto ipf = etl::make_inplace_function(&Object::member_void, object);

      ipf();

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Void_Called);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_void_compile_time)
    {
      auto ipf = etl::make_inplace_function<Object, &Object::member_void, object_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Member_Void_Called);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_const)
    {
      auto ipf = etl::inplace_function<void(void)>::create<Object, &Object::member_void_const, const_object_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Member_Void_Const_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_void_const_run_time)
    {
      static const Object object;

      auto ipf = etl::make_inplace_function(&Object::member_void_const, object);

      ipf();

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Void_Const_Called);
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_void_const_compile_time)
    {
      auto ipf = etl::make_inplace_function<Object, &Object::member_void_const, const_object_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Member_Void_Const_Called);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int)
    {
      auto ipf = etl::inplace_function<int(int, int)>::create<Object, &Object::member_int, object_static>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_int_run_time)
    {
      Object object;

      auto ipf = etl::make_inplace_function(&Object::member_int, object); // Pass it by value to the inplace function.

      ipf(VALUE1, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_int_compile_time)
    {
      auto ipf = etl::make_inplace_function<Object, &Object::member_int, object_static>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_const)
    {
      auto ipf = etl::inplace_function<int(int, int)>::create<Object, &Object::member_int_const, object_static>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Int_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_int_const_run_time)
    {
      Object object;

      auto ipf = etl::make_inplace_function(&Object::member_int_const, object);

      ipf(VALUE1, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Int_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
#if ETL_USING_CPP17
    TEST_FIXTURE(SetupFixture, test_make_inplace_function_member_int_const_compile_time)
    {
      auto ipf = etl::make_inplace_function<Object, &Object::member_int_const, object_static>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Int_Const_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference)
    {
      Object object;
      auto ipf = etl::inplace_function<void(const Data&, int)>(&Object::member_reference, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_member_reference_run_time)
    {
      Object object;
      auto ipf = etl::make_inplace_function(&Object::member_reference, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_member_reference_compile_time)
    {
      auto ipf = etl::make_inplace_function<Object, &Object::member_reference, object_static>();

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_const)
    {
      Object object;
      auto ipf = etl::inplace_function<void(const Data&, int)>(&Object::member_reference_const, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_member_reference_run_time_const)
    {
      Object object;
      auto ipf = etl::make_inplace_function(&Object::member_reference_const, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_member_reference_compile_time_const)
    {
      auto ipf = etl::make_inplace_function<Object, &Object::member_reference_const, object_static>();

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_moveableonly)
    {
      Object object;
      auto ipf = etl::inplace_function<void(MoveableOnlyData&&)>(&Object::member_moveableonly, object);

      MoveableOnlyData data;
      data.ipf = VALUE1;

      ipf(std::move(data));

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Moveableonly_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_static)
    {
      auto ipf = etl::inplace_function<void(const Data&, int)>(Object::member_static);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Static_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_member_static_run_time)
    {
      auto ipf = etl::make_inplace_function(Object::member_static);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Static_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_make_member_static_compile_time)
    {
      auto ipf = etl::make_inplace_function<Object::member_static>();

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Static_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_free_int_run_time)
    {
      // Start with a destructible object to check that it is properly destroyed
      DestructibleMovableObject object;
      etl::inplace_function<void(int, int)> ipf(object);

      ipf.set(free_int);

      CHECK_TRUE(DestructibleMovableObject::destructor_called);

      ipf(VALUE1, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_free_int_compile_time)
    {
      etl::inplace_function<void(int, int)> ipf;

      ipf.set<&free_int>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Free_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_lambda_int_run_time)
    {
      etl::inplace_function<void(int, int)> ipf;

      auto lambda = [](int i, int j) { function_called = FunctionCalled::Lambda_Called; parameter_correct = (i == VALUE1) && (j == VALUE2); };

      ipf.set(lambda);

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_lambda_int_compile_time)
    {
      etl::inplace_function<void(int, int)> ipf;

      ipf.set<global_lambda>();

      ipf(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Lambda_Called);
      CHECK(parameter_correct);
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_functor_run_time)
    {
      etl::inplace_function<void(void)> ipf;

      Functor functor;

      ipf.set(functor);

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_functor_compile_time)
    {
      etl::inplace_function<void(void)> ipf;

      ipf.set<Functor, functor_static>();

      ipf();

      CHECK(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_run_time)
    {
      Object object;
      etl::inplace_function<void(const Data&, int)> ipf;

      ipf.set(&Object::member_reference, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_compile_time)
    {
      Object object;
      etl::inplace_function<void(const Data&, int)> ipf;

      ipf.set(&Object::member_reference, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Reference_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_const_run_time)
    {
      const Object object;
      etl::inplace_function<void(const Data&, int)> ipf;

      ipf.set(&Object::member_reference_const, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_set_member_reference_const_compile_time)
    {
      const Object object;
      etl::inplace_function<void(const Data&, int)> ipf;

      ipf.set(&Object::member_reference_const, object);

      Data data;
      data.ipf = VALUE1;

      ipf(data, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Reference_Const_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_construct_run_time_contruction)
    {
      Object object;

      auto d1 = etl::inplace_function<int(int, int)>(&Object::member_int, object);
      auto d2(d1);

      d2(VALUE1, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_construct_compile_time_contruction)
    {
      auto d1 = etl::inplace_function<int(int, int)>::create<Object, &Object::member_int, object_static>();
      auto d2(d1);

      d2(VALUE1, VALUE2);

      CHECK(function_called == FunctionCalled::Member_Int_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_construct_different_size)
    {
      auto d1 = etl::inplace_function<void(int, int),  8, 16>(free_int);
      auto d2 = etl::inplace_function<void(int, int), 16, 16>(free_int);
      auto d3 = etl::inplace_function<void(int, int), 16,  8>(free_int);
      auto d4 = etl::inplace_function<void(int, int),  8,  8>(free_int);

      // These should not trigger any static asserts.
      decltype(d1) d1_1_copy(d1);
      decltype(d1) d1_4_copy(d4);

      decltype(d2) d2_1_copy(d1);
      decltype(d2) d2_2_copy(d2);

      decltype(d3) d3_3_copy(d3);
      decltype(d3) d3_4_copy(d4);

      CHECK_EQUAL(d1.size(), d1_1_copy.size());
      CHECK_EQUAL(d1.alignment(), d1_1_copy.alignment());

      CHECK_EQUAL(d1.size(), d1_4_copy.size());
      CHECK_EQUAL(d1.alignment(), d1_4_copy.alignment());

      CHECK_EQUAL(d2.size(), d2_1_copy.size());
      CHECK_EQUAL(d2.alignment(), d2_1_copy.alignment());

      CHECK_EQUAL(d2.size(), d2_2_copy.size());
      CHECK_EQUAL(d2.alignment(), d2_2_copy.alignment());

      CHECK_EQUAL(d3.size(), d3_3_copy.size());
      CHECK_EQUAL(d3.alignment(), d3_3_copy.alignment());

      CHECK_EQUAL(d3.size(), d3_4_copy.size());
      CHECK_EQUAL(d3.alignment(), d3_4_copy.alignment());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_copy_assignment)
    {
      DestructibleMovableObject object;

      auto d1 = etl::inplace_function<void(int, int)>(object);
      etl::inplace_function<void(int, int)> d2;

      d2 = d1;

      d2(VALUE1, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK_TRUE(DestructibleMovableObject::function_operator_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_move_assignment)
    {
      DestructibleMovableObject object;

      auto d1 = etl::inplace_function<void(int, int)>(object);
      etl::inplace_function<void(int, int)> d2;

      d2 = etl::move(d1);

      d2(VALUE1, VALUE2);

      CHECK_EQUAL(0, object.call_count);  // Ensure the original object is not modified.
      CHECK_TRUE(DestructibleMovableObject::function_operator_called);
    }

    //*************************************************************************
    TEST(test_vector_of_inplace_functions)
    {
      etl::vector<etl::inplace_function<int(int)>, 5> vector_of_inplace_functions;

      vector_of_inplace_functions.push_back(etl::inplace_function<int(int)>::create<times_2>());

      CHECK_EQUAL(42, vector_of_inplace_functions.front()(21));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_run_time_normal)
    {
      auto ipf = etl::inplace_function<int(int, int)>(normal);

      int result = ipf.call_or(alternative, VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_run_time_alternative)
    {
      etl::inplace_function<int(int, int)> ipf;

      int result = ipf.call_or(alternative, VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_compile_time_alternative)
    {
      etl::inplace_function<int(int, int)> ipf;

      int result = ipf.call_or<alternative>(VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_or_inplace_function_alternative)
    {
      etl::inplace_function<int(int, int)> ipf;

      auto alt = etl::inplace_function<int(int, int)>::create<alternative>();

      int result = ipf.call_or(alt, VALUE1, VALUE2);

      CHECK_EQUAL(VALUE1 + VALUE2 + 1, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_valid)
    {
      auto ipf = etl::inplace_function<int(int, int)>::create<normal>();

      etl::optional<int> result = ipf.call_if(VALUE1, VALUE2);

      CHECK(bool(result));
      CHECK_EQUAL(VALUE1 + VALUE2, result.value());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_not_valid)
    {
      etl::inplace_function<int(int, int)> ipf;

      etl::optional<int> result = ipf.call_if(VALUE1, VALUE2);

      CHECK(!bool(result));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_valid_returning_void)
    {
      auto ipf = etl::inplace_function<void(int, int)>::create<normal_returning_void>();

      bool was_called = ipf.call_if(VALUE1, VALUE2);

      CHECK(was_called);
      CHECK(function_called == FunctionCalled::Normal_Returning_Void_Called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_call_if_and_not_valid_returning_void)
    {
      etl::inplace_function<void(int, int)> ipf;

      bool was_called = ipf.call_if(VALUE1, VALUE2);

      CHECK(!was_called);
      CHECK(function_called == FunctionCalled::Not_Called);
      CHECK(!parameter_correct);
    }
    
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_inplace_function_for)
    {
      FunctorSized<32> functor32;

      auto ipf = etl::inplace_function_for<int(void), decltype(functor32)>(functor32);

      int size = 0;

      function_called = FunctionCalled::Not_Called;
      size = ipf();
      CHECK_EQUAL(functor32.size(), size);
      CHECK_TRUE(function_called == FunctionCalled::Operator_Called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_inplace_function_for_any)
    {
      FunctorSized<8>  functor8;
      FunctorSized<16> functor16;
      FunctorSized<32> functor32;

      auto ipf = etl::inplace_function_for_any<int(void), FunctorSized<8>, FunctorSized<16>, FunctorSized<32>>();
      int size = 0;

      function_called = FunctionCalled::Not_Called;
      ipf.set(functor8);
      size = ipf();
      CHECK_TRUE(function_called == FunctionCalled::Operator_Called);
      CHECK_EQUAL(functor8.size(), size);

      function_called = FunctionCalled::Not_Called;
      ipf.set(functor16);
      size = ipf();
      CHECK_TRUE(function_called == FunctionCalled::Operator_Called);
      CHECK_EQUAL(functor16.size(), size);
      
      function_called = FunctionCalled::Not_Called;
      ipf.set(functor32);
      size = ipf();
      CHECK_TRUE(function_called == FunctionCalled::Operator_Called);
      CHECK_EQUAL(functor32.size(), size);
    }

    //*************************************************************************
    TEST(make_inplace_function_from_free_function_name)
    {
      // Create from function name (decays to function pointer)
      auto ipf = etl::make_inplace_function<int(int, int)>(&free_int_return);

      CHECK_TRUE(bool(ipf));
      CHECK_EQUAL(7, ipf(3, 4));
    }

    //*************************************************************************
    TEST(make_inplace_function_from_free_function_pointer_variable)
    {
      // Create from explicit function pointer variable
      int (*fp)(int, int) = &free_int_return;

      auto ipf = etl::make_inplace_function<int(int, int)>(fp);

      CHECK_TRUE(ipf.is_valid());
      CHECK_EQUAL(13, ipf(6, 7));
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_assignment_to_an_ipf_of_function_over_a_lambda_and_back)
    {
      etl::inplace_function<int(int i, int j)> ipf;

      int value = 1;
      auto lambda = [value](int i, int j) -> int { return value + i + j; };

      int result = 0;

      ipf = lambda;
      result = ipf(2, 3);
      CHECK_EQUAL(1 + 2 + 3, result);

      ipf = normal;
      result = ipf(3, 4);
      CHECK_EQUAL(3 + 4, result);

      ipf = lambda;
      result = ipf(5, 6);
      CHECK_EQUAL(1 + 5 + 6, result);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_valid_valid)
    {
      Object object;

      // ipf1 -> normal, ipf2 -> member_int
      etl::inplace_function<int(int, int)> ipf1(normal);
      etl::inplace_function<int(int, int)> ipf2(&Object::member_int, object);

      int r1_before = ipf1(VALUE1, VALUE2);
      int r2_before = ipf2(VALUE1, VALUE2);
      CHECK_EQUAL(VALUE1 + VALUE2,     r1_before);
      CHECK_EQUAL(VALUE1 + VALUE2 + 1, r2_before);

      swap(ipf1, ipf2); // ADL swap uses member swap

      int r1_after = ipf1(VALUE1, VALUE2);
      int r2_after = ipf2(VALUE1, VALUE2);
      // After swap ipf1 should now hold 'member_int', ipf2 should hold 'normal'
      CHECK_EQUAL(VALUE1 + VALUE2 + 1, r1_after);
      CHECK_EQUAL(VALUE1 + VALUE2,     r2_after);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_valid_empty)
    {
      etl::inplace_function<int(int, int)> ipf_valid(normal);
      etl::inplace_function<int(int, int)> ipf_empty; // default constructed

      CHECK_TRUE(ipf_valid.is_valid());
      CHECK_FALSE(ipf_empty.is_valid());

      swap(ipf_valid, ipf_empty);

      // ipf_valid should now be empty, ipf_empty should now have 'normal'
      CHECK_FALSE(ipf_valid.is_valid());
      CHECK_TRUE(ipf_empty.is_valid());

      CHECK_EQUAL(VALUE1 + VALUE2, ipf_empty(VALUE1, VALUE2));
      CHECK_THROW(ipf_valid(VALUE1, VALUE2), etl::inplace_function_uninitialized);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_empty_empty)
    {
      etl::inplace_function<int(int, int)> ipf_empty1; // default constructed
      etl::inplace_function<int(int, int)> ipf_empty2; // default constructed

      CHECK_FALSE(ipf_empty1.is_valid());
      CHECK_FALSE(ipf_empty2.is_valid());

      swap(ipf_empty1, ipf_empty2);

      // Both should still be empty
      CHECK_FALSE(ipf_empty1.is_valid());
      CHECK_FALSE(ipf_empty2.is_valid());
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_self_noop)
    {
      etl::inplace_function<int(int, int)> ipf(normal);
      int before = ipf(VALUE1, VALUE2);
      ipf.swap(ipf); // self-swap should be a no-op
      int after  = ipf(VALUE1, VALUE2);
      CHECK_EQUAL(before, after);
      CHECK_EQUAL(VALUE1 + VALUE2, after);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_swap_runtime_functor_and_function)
    {
      struct PlusOne
      {
        int operator()(int a, int b) { return a + b + 1; }
      } functor;

      etl::inplace_function<int(int, int)> ipf_functor(functor);
      etl::inplace_function<int(int, int)> ipf_function(normal);

      CHECK_EQUAL(VALUE1 + VALUE2 + 1, ipf_functor(VALUE1, VALUE2));
      CHECK_EQUAL(VALUE1 + VALUE2,     ipf_function(VALUE1, VALUE2));

      ipf_functor.swap(ipf_function);

      CHECK_EQUAL(VALUE1 + VALUE2,     ipf_functor(VALUE1, VALUE2));     // now holds 'normal'
      CHECK_EQUAL(VALUE1 + VALUE2 + 1, ipf_function(VALUE1, VALUE2));    // now holds functor
    }

#if defined(ETL_NEGATIVE_TEST_INPLACE_FUNCTION_BAD_RETURN)
    //*************************************************************************
    // Triggers: return type not convertible (void -> int)
    TEST(test_inplace_function_static_assert_bad_return)
    {
      auto bad = [](int) { /* returns void */ };
      // static_assert in lambda_stub/const_lambda_stub should trigger:
      // "etl::inplace_function: bound lambda/functor is not compatible with the inplace_function signature"
      auto ipf = etl::inplace_function<int(int)>(bad);
      (void)ipf;
    }
#endif

#if defined(ETL_NEGATIVE_TEST_INPLACE_FUNCTION_RVALUE_PARAM_MISMATCH_NONCONST)
    //*************************************************************************
    // Triggers: parameter ref-qualification mismatch (expects rvalue, lambda takes lvalue ref)
    TEST(test_inplace_function_static_assert_param_mismatch_nonconst)
    {
      auto bad = [](int&) { /* needs lvalue */ };
      // Not invocable with int&&, so is_compatible_callable is false, so static_assert triggers
      auto ipf = etl::inplace_function<void(int&&)>(bad);
      (void)ipf;
    }
#endif

#if defined(ETL_NEGATIVE_TEST_INPLACE_FUNCTION_RVALUE_PARAM_MISMATCH_CONST)
    //*************************************************************************
    // Same as above, but binds a const lambda to hit const_lambda_stub
    TEST(test_inplace_function_static_assert_param_mismatch_const)
    {
      const auto bad = [](int&) { /* needs lvalue */ };
      // Not invocable with int&&, so is_compatible_callable is false, so static_assert triggers
      auto ipf = etl::inplace_function<void(int&&)>(bad);
      (void)ipf;
    }
#endif
  }
}

