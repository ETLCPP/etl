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

#include "etl/delegate.h"
#include "etl/delegate_service.h"

#if defined(ETL_DELEGATE_FORCE_CPP03_IMPLEMENTATION)

namespace
{
  const size_t Size   = 3UL;
  const size_t Offset = 5UL;

  using Service = etl::delegate_service<Size, Offset>;

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
      : callback(Service::delegate_type::create<Object, &Object::member1>(*this))
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
    Service::delegate_type callback;
  };

  Object object;

  // Callback for 'member2'.
  Service::delegate_type member_callback = Service::delegate_type::create<Object, object, &Object::member2>();

  // Callback for 'global'.
  Service::delegate_type global_callback = Service::delegate_type::create<global>();

  // Callback for 'unhandled'.
  Service::delegate_type unhandled_callback = Service::delegate_type::create<unhandled>();

  //*****************************************************************************
  // Initialises the test results.
  //*****************************************************************************
  struct SetupFixture
  {
    SetupFixture()
    {
      called_id        = UINT_MAX;
      global_called    = false;
      member1_called   = false;
      member2_called   = false;
      unhandled_called = false;
    }
  };
}

namespace
{
  enum
  {
    Global = Offset,
    Member1,
    Member2,
    Out_Of_Range
  };

  SUITE(test_delegate_service)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_global_compile_time)
    {
      Service service;

      service.register_delegate<Global>(global_callback);
      service.register_delegate<Member1>(object.callback);
      service.register_delegate<Member2>(member_callback);

      service.call<Global>();

      CHECK_EQUAL(Global, called_id);
      CHECK(global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_global_run_time)
    {
      Service service;

      service.register_delegate(Global,  global_callback);
      service.register_delegate(Member1, object.callback);
      service.register_delegate(Member2, member_callback);

      service.call(Global);

      CHECK_EQUAL(Global, called_id);
      CHECK(global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_member1_compile_time)
    {
      Service service;

      service.register_delegate<Global>(global_callback);
      service.register_delegate<Member1>(object.callback);
      service.register_delegate<Member2>(member_callback);

      service.call<Member1>();

      CHECK_EQUAL(Member1, called_id);
      CHECK(!global_called);
      CHECK(member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_member1_run_time)
    {
      Service service;

      service.register_delegate(Global,  global_callback);
      service.register_delegate(Member1, object.callback);
      service.register_delegate(Member2, member_callback);

      service.call(Member1);

      CHECK_EQUAL(Member1, called_id);
      CHECK(!global_called);
      CHECK(member1_called);
      CHECK(!member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_member2_compile_time)
    {
      Service service;

      service.register_delegate<Global>(global_callback);
      service.register_delegate<Member1>(object.callback);
      service.register_delegate<Member2>(member_callback);

      service.call<Member2>();

      CHECK_EQUAL(Member2, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(member2_called);
      CHECK(!unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_out_of_range_run_time_default)
    {
      Service service;

      service.register_delegate<Global>(global_callback);
      service.register_delegate<Member1>(object.callback);
      service.register_delegate<Member2>(member_callback);

      service.call(Out_Of_Range);

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

      service.register_delegate<Global>(global_callback);
      service.register_delegate<Member1>(object.callback);
      service.register_delegate<Member2>(member_callback);

      service.register_unhandled_delegate(unhandled_callback);

      service.call(Out_Of_Range);

      CHECK_EQUAL(Out_Of_Range, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_not_registered_compile_time_default)
    {
      Service service;

      service.register_delegate<Global>(global_callback);
      service.register_delegate<Member2>(member_callback);

      service.call<Member1>();

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

      service.register_delegate(Global,  global_callback);
      service.register_delegate(Member2, member_callback);

      service.call(Member1);

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

      service.register_delegate<Global>(global_callback);
      service.register_delegate<Member2>(member_callback);

      service.register_unhandled_delegate(unhandled_callback);

      service.call<Member1>();

      CHECK_EQUAL(Member1, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_unhandled_run_time_user_supplied)
    {
      Service service;

      service.register_delegate(Global,  global_callback);
      service.register_delegate(Member2, member_callback);

      service.register_unhandled_delegate(unhandled_callback);

      service.call(Member1);

      CHECK_EQUAL(Member1, called_id);
      CHECK(!global_called);
      CHECK(!member1_called);
      CHECK(!member2_called);
      CHECK(unhandled_called);
    }
  };
}

#endif
