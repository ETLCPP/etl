/******************************************************************************
The MIT License(MIT)

Embedded Template Library.

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

#include <UnitTest++/UnitTest++.h>

#include "../function.h"

//*****************************************************************************
const int VALUE = 1;
bool functionCalled   = false;
bool parameterCorrect = false;

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
void Call(etl::ifunction<void>& function)
{
  function();
}

//*****************************************************************************
// Call the function taking an int parameter.
//*****************************************************************************
void Call(etl::ifunction<int>& function)
{
  function(VALUE);
}

//*****************************************************************************
// Call the function taking a Data parameter.
//*****************************************************************************
void Call(etl::ifunction<const Data&>& function)
{
  Data data;
  data.d = VALUE;
  function(data);
}

//*****************************************************************************
// The free function taking no parameters.
//*****************************************************************************
void FreeVoid()
{
  functionCalled = true;
}

//*****************************************************************************
// The free function taking an int parameter.
//*****************************************************************************
void FreeInt(int i)
{
  functionCalled = true;
  parameterCorrect = (i == VALUE);
}

//*****************************************************************************
// The free function taking a Data reference parameter.
//*****************************************************************************
void FreeReference(const Data& data)
{
  functionCalled = true;
  parameterCorrect = (data.d == VALUE);
}

//*****************************************************************************
// The test class with member functions.
//*****************************************************************************
class Test
{
public:

  void MemberVoid()
  {
    functionCalled = true;
  }

  void MemberInt(int i)
  {
    functionCalled = true;
    parameterCorrect = (i == VALUE);
  }

  void MemberReference(const Data& data)
  {
    functionCalled = true;
    parameterCorrect = (data.d == VALUE);
  }
};

//*****************************************************************************
// Initialises the test results.
//*****************************************************************************
struct SetupFixture
{
  SetupFixture()
  {
    functionCalled   = false;
    parameterCorrect = false;
  }
};

namespace
{		
  SUITE(TestFunction)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, TestFreeVoid)
    {
      etl::function<void, void> function(FreeVoid);

      Call(function);

      CHECK(functionCalled);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, TestFreeInt)
    {
      etl::function<void, int> function(FreeInt);

      Call(function);

      CHECK(functionCalled);
      CHECK(parameterCorrect);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, TestFreeReference)
    {
      etl::function<void, const Data&> function(FreeReference);

      Call(function);

      CHECK(functionCalled);
      CHECK(parameterCorrect);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, TestMemberVoid)
    {
      Test test;
      etl::function<Test, void> function(test, &Test::MemberVoid);

      Call(function);

      CHECK(functionCalled);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, TestMemberInt)
    {
      Test test;
      etl::function<Test, int> function(test, &Test::MemberInt);

      Call(function);

      CHECK(functionCalled);
      CHECK(parameterCorrect);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, TestMemberReference)
    {
      Test test;
      etl::function<Test, const Data&> function(test, &Test::MemberReference);

      Call(function);

      CHECK(functionCalled);
      CHECK(parameterCorrect);
    }
  };
}