/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include "UnitTest++.h"

#include "function.h"

//*****************************************************************************
const int VALUE = 1;
bool function_called   = false;
bool parameter_correct = false;

//*****************************************************************************
// Test data structure.
//*****************************************************************************
struct Data
{
  int d;
};

//*****************************************************************************
// Call the function taking no parameters.
//*****************************************************************************
void call(etl::ifunction<void>& function)
{
  function();
}

//*****************************************************************************
// Call the function taking an int parameter.
//*****************************************************************************
void call(etl::ifunction<int>& function)
{
  function(VALUE);
}

//*****************************************************************************
// Call the function taking a Data parameter.
//*****************************************************************************
void call(etl::ifunction<const Data&>& function)
{
  Data data;
  data.d = VALUE;
  function(data);
}

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
void free_int(int i)
{
  function_called   = true;
  parameter_correct = (i == VALUE);
}

//*****************************************************************************
// The free function taking a Data reference parameter.
//*****************************************************************************
void free_reference(const Data& data)
{
  function_called   = true;
  parameter_correct = (data.d == VALUE);
}

//*****************************************************************************
// The test class with member functions.
//*****************************************************************************
class Test
{
public:

  void member_void()
  {
    function_called = true;
  }

  void member_int(int i)
  {
    function_called = true;
    parameter_correct = (i == VALUE);
  }

  void member_reference(const Data& data)
  {
    function_called = true;
    parameter_correct = (data.d == VALUE);
  }
};

Test test_static;

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

namespace
{		
  SUITE(test_function)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_void)
    {
      etl::function<void, void> function(free_void);

      call(function);

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_int)
    {
      etl::function<void, int> function(free_int);

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_reference)
    {
      etl::function<void, const Data&> function(free_reference);

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_void_compile_time)
    {
      etl::function_fv<free_void> function;

      call(function);

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_int_compile_time)
    {
      etl::function_fp<int, free_int> function;

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_free_reference_compile_time)
    {
      etl::function_fp<const Data&, free_reference> function;

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void)
    {
      Test test;
      etl::function<Test, void> function(test, &Test::member_void);

      call(function);

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int)
    {
      Test test;
      etl::function<Test, int> function(test, &Test::member_int);

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference)
    {
      Test test;
      etl::function<Test, const Data&> function(test, &Test::member_reference);

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_void_compile_time)
    {
      Test test;
      etl::function_mv<Test, &Test::member_void> function(test);

      call(function);

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_int_compile_time)
    {
      Test test;
      etl::function_mp<Test, int, &Test::member_int> function(test);

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_member_reference_compile_time)
    {
      Test test;
      etl::function_mp<Test, const Data&, &Test::member_reference> function(test);

      call(function);

      CHECK(function_called);
      CHECK(parameter_correct);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_instance_member_void_compile_time)
    {
      function_called = false;

      etl::function_imv<Test, test_static, &Test::member_void> function;

      call(function);

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_instance_member_parameter_compile_time)
    {
      function_called = false;

      etl::function_imp<Test, int, test_static, &Test::member_int> function;

      call(function);

      CHECK(function_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_instance_member_reference_compile_time)
    {
      function_called = false;

      etl::function_imp<Test, const Data&, test_static, &Test::member_reference> function;

      call(function);

      CHECK(function_called);
    }
  };
}