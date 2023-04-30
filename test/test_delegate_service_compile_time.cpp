/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 John Wellbelove

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

#include "etl/delegate.h"
#include "etl/delegate_service.h"

#if !defined(ETL_DELEGATE_FORCE_CPP03_IMPLEMENTATION)

namespace
{
  const size_t Size   = 2UL;
  const size_t Offset = 5UL;

  using Service = etl::delegate_service<Size, Offset>;

  //*****************************************************************************
  bool global_called    = false;
  bool member_called    = false;
  bool unhandled_called = false;
  size_t called_id      = UINT_MAX;

  //*****************************************************************************
  // The global function taking no parameters.
  //*****************************************************************************
  void global(size_t id)
  {
    global_called = true;
    called_id     = id;
  }

  //*****************************************************************************
  // The external unhandled callback.
  //*****************************************************************************
  void unhandled(size_t id)
  {
    unhandled_called = true;
    called_id        = id;
  }

  //*****************************************************************************
  // The test class with member functions.
  //*****************************************************************************
  class Test
  {
  public:

    Test()
    {
    }

    void member(size_t id)
    {
      member_called = true;
      called_id      = id;
    }
  };

  Test test;

  // Callback for 'member'.
  ETL_CONSTEXPR14 etl::delegate<void(size_t)> member_callback = etl::delegate<void(size_t)>::create<Test, test, &Test::member>();

  // Callback for 'global'.
  ETL_CONSTEXPR14 etl::delegate<void(size_t)> global_callback = etl::delegate<void(size_t)>::create<global>();

  // Callback for 'unhandled'.
  ETL_CONSTEXPR14 etl::delegate<void(size_t)> unhandled_callback = etl::delegate<void(size_t)>::create<unhandled>();

  ETL_CONSTEXPR14 etl::delegate<void(size_t)> delegate_list[]
  {
    global_callback,
    member_callback,
    unhandled_callback
  };

  //*****************************************************************************
  // Initialises the test results.
  //*****************************************************************************
  struct SetupFixture
  {
    SetupFixture()
    {
      called_id        = UINT_MAX;
      global_called    = false;
      member_called    = false;
      unhandled_called = false;
    }
  };
}

namespace
{
  enum
  {
    Global = Offset,
    Member,
    Out_Of_Range
  };

  SUITE(test_delegate_service)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_compile_time_constexpr)
    {
      etl::delegate_service<Size, Offset, delegate_list> service;

      service.call<Global>();
      CHECK_EQUAL(Global, called_id);
      CHECK(global_called);
      CHECK(!member_called);
      CHECK(!unhandled_called);

      called_id = UINT_MAX;
      global_called = false;
      member_called = false;
      unhandled_called = false;

      service.call<Member>();
      CHECK_EQUAL(Member, called_id);
      CHECK(!global_called);
      CHECK(member_called);
      CHECK(!unhandled_called);

      called_id = UINT_MAX;
      global_called = false;
      member_called = false;
      unhandled_called = false;

      service.call(Out_Of_Range);
      CHECK_EQUAL(Out_Of_Range, called_id);
      CHECK(!global_called);
      CHECK(!member_called);
      CHECK(unhandled_called);
    }
  };
}

#endif
