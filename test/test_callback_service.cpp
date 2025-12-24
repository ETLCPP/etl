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

#include "etl/function.h"
#include "etl/callback_service.h"

namespace
{
  const size_t SIZE   = 3UL;
  const size_t OFFSET = 5UL;

  typedef etl::callback_service<SIZE, OFFSET> Service;

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
  class Object
  {
  public:

    Object()
      : callback(*this)
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
    etl::function_mp<Object, size_t, &Object::member1> callback;
  };

  Object object;

  // Callback for 'member2'.
  etl::function_imp<Object, size_t, object, &Object::member2> member_callback;

  // Callback for 'global'.
  etl::function_fp<size_t, global> global_callback;

  // Callback for 'unhandled'.
  etl::function_fp<size_t, unhandled> unhandled_callback;

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

  SUITE(test_callback_service)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_global_compile_time)
    {
      Service service;

      service.register_callback<GLOBAL>(global_callback);
      service.register_callback<MEMBER1>(object.callback);
      service.register_callback<MEMBER2>(member_callback);

      service.callback<GLOBAL>();

      CHECK_EQUAL(GLOBAL, called_id);
      CHECK(global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_global_run_time)
    {
      Service service;

      service.register_callback(GLOBAL,  global_callback);
      service.register_callback(MEMBER1, object.callback);
      service.register_callback(MEMBER2, member_callback);

      service.callback(GLOBAL);

      CHECK_EQUAL(GLOBAL, called_id);
      CHECK(global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_member1_compile_time)
    {
      Service service;

      service.register_callback<GLOBAL>(global_callback);
      service.register_callback<MEMBER1>(object.callback);
      service.register_callback<MEMBER2>(member_callback);

      service.callback<MEMBER1>();

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_member1_run_time)
    {
      Service service;

      service.register_callback(GLOBAL,  global_callback);
      service.register_callback(MEMBER1, object.callback);
      service.register_callback(MEMBER2, member_callback);

      service.callback(MEMBER1);

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_member2_compile_time)
    {
      Service service;

      service.register_callback<GLOBAL>(global_callback);
      service.register_callback<MEMBER1>(object.callback);
      service.register_callback<MEMBER2>(member_callback);

      service.callback<MEMBER2>();

      CHECK_EQUAL(MEMBER2, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_unhandled_out_of_range_run_time_default)
    {
      Service service;

      service.register_callback<GLOBAL>(global_callback);
      service.register_callback<MEMBER1>(object.callback);
      service.register_callback<MEMBER2>(member_callback);

      service.callback(OUT_OF_RANGE);

      CHECK_EQUAL(UINT_MAX, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_unhandled_out_of_range_run_time_user_supplied)
    {
      Service service;

      service.register_callback<GLOBAL>(global_callback);
      service.register_callback<MEMBER1>(object.callback);
      service.register_callback<MEMBER2>(member_callback);

      service.register_unhandled_callback(unhandled_callback);

      service.callback(OUT_OF_RANGE);

      CHECK_EQUAL(OUT_OF_RANGE, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_unhandled_not_registered_compile_time_default)
    {
      Service service;

      service.register_callback<GLOBAL>(global_callback);
      service.register_callback<MEMBER2>(member_callback);

      service.callback<MEMBER1>();

      CHECK_EQUAL(UINT_MAX, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_unhandled_not_registered_run_time_default)
    {
      Service service;

      service.register_callback(GLOBAL,  global_callback);
      service.register_callback(MEMBER2, member_callback);

      service.callback(MEMBER1);

      CHECK_EQUAL(UINT_MAX, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_not_registered_compile_time_user_supplied)
    {
      Service service;

      service.register_callback<GLOBAL>(global_callback);
      service.register_callback<MEMBER2>(member_callback);

      service.register_unhandled_callback(unhandled_callback);

      service.callback<MEMBER1>();

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_callback_unhandled_run_time_user_supplied)
    {
      Service service;

      service.register_callback(GLOBAL,  global_callback);
      service.register_callback(MEMBER2, member_callback);

      service.register_unhandled_callback(unhandled_callback);

      service.callback(MEMBER1);

      CHECK_EQUAL(MEMBER1, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }
  };
}
