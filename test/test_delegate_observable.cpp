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

#include "etl/delegate.h"
#include "etl/delegate_observable.h"

#if !defined(ETL_DELEGATE_FORCE_CPP03_IMPLEMENTATION)

namespace
{
  const size_t Max_Observers = 2UL;

  using Notification = int;
  using Observable   = etl::delegate_observable<Notification, Max_Observers>;
  using Delegate     = Observable::delegate_type;

  //*****************************************************************************
  bool global_called = false;
  bool member_called = false;
  int  global_value  = 0;
  int  member_value  = 0;

  //*****************************************************************************
  // The global function taking no parameters.
  //*****************************************************************************
  static void global(int id)
  {
    global_called = true;
    global_value  = id;
  }

  //*****************************************************************************
  // The test class with member functions.
  //*****************************************************************************
  class Object
  {
  public:

    ETL_CONSTEXPR14 Object()
    {
    }

    void member(int id)
    {
      member_called = true;
      member_value  = id;
    }
  };

  Object object;

  // Callback for 'member1'.
  ETL_CONSTEXPR14 Delegate member_callback = Delegate::create<Object, &Object::member, object>();

  // Callback for 'global1'.
  ETL_CONSTEXPR14 Delegate global_callback = Delegate::create<global>();

  //*****************************************************************************
  // Initialises the test results.
  //*****************************************************************************
  struct SetupFixture
  {
    SetupFixture()
    {
      global_called = false;
      member_called = false;
      global_value  = 0;
      member_value  = 0;
    }
  };

#if defined(ETL_COMPILER_GCC)
  #if (ETL_COMPILER_FULL_VERSION <= 1100)
      #define ETL_GCC_11 1
    #else
      #define ETL_GCC_11 0
  #endif
#else
  #define ETL_GCC_11 0
#endif

#define ETL_ENABLE_DELEGATE_OBSERVABLE_CREATE_OBSERVABLE (ETL_GCC_11 && ETL_USING_CPP14)

#if ETL_ENABLE_DELEGATE_OBSERVABLE_CREATE_OBSERVABLE
  //*************************************************************************
  ETL_CONSTEXPR14 Observable CreateObservable()
  {
    Observable observable;

    observable.add_observer(global_callback);
    observable.add_observer(member_callback);

    observable.remove_observer(global_callback);

    return observable; 
  }
#endif
}

namespace
{
  SUITE(test_delegate_service)
  {
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constexpr_delegate_observable)
    {
      static ETL_CONSTEXPR14 Observable observable(global_callback, member_callback);

      CHECK_EQUAL(2, observable.number_of_observers());

      observable.notify_observers(1);

      CHECK_TRUE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(1, global_value);
      CHECK_EQUAL(1, member_value); 

      //*********************************
      global_called = false;
      member_called = false;

      observable.notify_observers(2);

      CHECK_TRUE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(2, global_value);
      CHECK_EQUAL(2, member_value); 
    }

#if ETL_USING_CPP17
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constexpr_delegate_observable_using_template_deduction_guide_cpp17)
    {
      static ETL_CONSTEXPR14 etl::delegate_observable observable(int{}, global_callback, member_callback);

      CHECK_EQUAL(2, observable.number_of_observers());

      observable.notify_observers(1);

      CHECK_TRUE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(1, global_value);
      CHECK_EQUAL(1, member_value); 

      //*********************************
      global_called = false;
      member_called = false;

      observable.notify_observers(2);

      CHECK_TRUE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(2, global_value);
      CHECK_EQUAL(2, member_value); 
    }
#endif

#if ETL_ENABLE_DELEGATE_OBSERVABLE_CREATE_OBSERVABLE
    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_constexpr_delegate_observable_from_function)
    {
      static ETL_CONSTEXPR14 Observable observable = CreateObservable();

      CHECK_EQUAL(1, observable.number_of_observers());

      observable.notify_observers(1);

      CHECK_FALSE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(0, global_value);
      CHECK_EQUAL(1, member_value); 

      //*********************************
      global_called = false;
      member_called = false;

      observable.notify_observers(2);

      CHECK_FALSE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(0, global_value);
      CHECK_EQUAL(2, member_value); 
    }
#endif

    //*************************************************************************
    TEST_FIXTURE(SetupFixture, test_delegate_observable_add_and_remove)
    {
      Observable observable;
      CHECK_EQUAL(0, observable.number_of_observers());

      observable.notify_observers(1);
      CHECK_FALSE(global_called);
      CHECK_FALSE(member_called);
      CHECK_EQUAL(0, global_value);
      CHECK_EQUAL(0, member_value);

      //*********************************
      global_called = false;
      member_called = false;

      observable.add_observer(global_callback);
      CHECK_EQUAL(1, observable.number_of_observers());

      observable.notify_observers(2);
      CHECK_TRUE(global_called);
      CHECK_FALSE(member_called);
      CHECK_EQUAL(2, global_value);
      CHECK_EQUAL(0, member_value);
      
      //*********************************
      global_called = false;
      member_called = false;

      observable.add_observer(member_callback);
      CHECK_EQUAL(2, observable.number_of_observers());

      observable.notify_observers(3);
      CHECK_TRUE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(3, global_value);
      CHECK_EQUAL(3, member_value);

      //*********************************
      global_called = false;
      member_called = false;

      observable.remove_observer(global_callback);
      CHECK_EQUAL(1, observable.number_of_observers());

      observable.notify_observers(4);
      CHECK_FALSE(global_called);
      CHECK_TRUE(member_called);
      CHECK_EQUAL(3, global_value);
      CHECK_EQUAL(4, member_value);

      //*********************************
      global_called = false;
      member_called = false;

      observable.clear_observers();
      CHECK_EQUAL(0, observable.number_of_observers());

      observable.notify_observers(5);
      CHECK_FALSE(global_called);
      CHECK_FALSE(member_called);
      CHECK_EQUAL(3, global_value);
      CHECK_EQUAL(4, member_value);
    }
  };
}

#endif
