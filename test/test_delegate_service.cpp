/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2019 jwellbelove

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

#include "UnitTest++/UnitTest++.h"

#include "etl/delegate.h"
#include "etl/delegate_service.h"

namespace
{
  const size_t SIZE   = 3U;
  const size_t OFFSET = 5U;

  using Service = etl::delegate_service<SIZE, OFFSET>;

  //*****************************************************************************
  bool global_called    = false;
  bool member1_called   = false;
  bool member2_called   = false;
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
      : callback(etl::delegate<void(size_t)>::create<Test, &Test::member1>(*this))
    {
    }

    void member1(size_t id)
    {
      member1_called = true;
      called_id      = id;
    }

    void member2(size_t id)
    {
      member2_called = true;
      called_id      = id;
    }

    // Callback for 'member1'.
    etl::delegate<void(size_t)> callback;
  };

  Test test;

  // Callback for 'member2'.
  etl::delegate<void(size_t)> member_callback = etl::delegate<void(size_t)>::create<Test, test, &Test::member2>();

  // Callback for 'global'.
  etl::delegate<void(size_t)> global_callback = etl::delegate<void(size_t)>::create<global>();

  // Callback for 'unhandled'.
  etl::delegate<void(size_t)> unhandled_callback = etl::delegate<void(size_t)>::create<unhandled>();

  //*****************************************************************************
  // Initialises the test results.
  //*****************************************************************************
  struct SetupFixture
  {
    SetupFixture()
    {
      called_id = UINT_MAX;
      global_called = false;
      member1_called = false;
      member2_called = false;
      unhandled_called = false;
    }
  };
}

namespace
{
  enum
  {
    GLOBAL = OFFSET,
    MEMBER1,
    MEMBER2,
    OUT_OF_RANGE
  };

  SUITE(test_delegate_service)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_global_compile_time)
    {
      Service service;

      service.register_delegate<GLOBAL>(global_callback);
      service.register_delegate<MEMBER1>(test.callback);
      service.register_delegate<MEMBER2>(member_callback);

      service.call<GLOBAL>();

      CHECK_EQUAL(GLOBAL, called_id);
      CHECK(global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_global_run_time)
    {
      Service service;

      service.register_delegate(GLOBAL,  global_callback);
      service.register_delegate(MEMBER1, test.callback);
      service.register_delegate(MEMBER2, member_callback);

      service.call(GLOBAL);

      CHECK_EQUAL(GLOBAL, called_id);
      CHECK(global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_member1_compile_time)
    {
      Service service;

      service.register_delegate<GLOBAL>(global_callback);
      service.register_delegate<MEMBER1>(test.callback);
      service.register_delegate<MEMBER2>(member_callback);

      service.call<MEMBER1>();

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_member1_run_time)
    {
      Service service;

      service.register_delegate(GLOBAL,  global_callback);
      service.register_delegate(MEMBER1, test.callback);
      service.register_delegate(MEMBER2, member_callback);

      service.call(MEMBER1);

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_member2_compile_time)
    {
      Service service;

      service.register_delegate<GLOBAL>(global_callback);
      service.register_delegate<MEMBER1>(test.callback);
      service.register_delegate<MEMBER2>(member_callback);

      service.call<MEMBER2>();

      CHECK_EQUAL(MEMBER2, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_out_of_range_run_time_default)
    {
      Service service;

      service.register_delegate<GLOBAL>(global_callback);
      service.register_delegate<MEMBER1>(test.callback);
      service.register_delegate<MEMBER2>(member_callback);

      service.call(OUT_OF_RANGE);

      CHECK_EQUAL(UINT_MAX, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_out_of_range_run_time_user_supplied)
    {
      Service service;

      service.register_delegate<GLOBAL>(global_callback);
      service.register_delegate<MEMBER1>(test.callback);
      service.register_delegate<MEMBER2>(member_callback);

      service.register_unhandled_delegate(unhandled_callback);

      service.call(OUT_OF_RANGE);

      CHECK_EQUAL(OUT_OF_RANGE, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_not_registered_compile_time_default)
    {
      Service service;

      service.register_delegate<GLOBAL>(global_callback);
      service.register_delegate<MEMBER2>(member_callback);

      service.call<MEMBER1>();

      CHECK_EQUAL(UINT_MAX, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_not_registered_run_time_default)
    {
      Service service;

      service.register_delegate(GLOBAL,  global_callback);
      service.register_delegate(MEMBER2, member_callback);

      service.call(MEMBER1);

      CHECK_EQUAL(UINT_MAX, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_not_registered_compile_time_user_supplied)
    {
      Service service;

      service.register_delegate<GLOBAL>(global_callback);
      service.register_delegate<MEMBER2>(member_callback);

      service.register_unhandled_delegate(unhandled_callback);

      service.call<MEMBER1>();

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_run_time_user_supplied)
    {
      Service service;

      service.register_delegate(GLOBAL,  global_callback);
      service.register_delegate(MEMBER2, member_callback);

      service.register_unhandled_delegate(unhandled_callback);

      service.call(MEMBER1);

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }
  };
}
