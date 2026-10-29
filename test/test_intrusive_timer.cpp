/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 John Wellbelove

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
#include "etl/error_handler.h"
#include "etl/intrusive_timer.h"
#include "etl/message.h"
#include "etl/message_router.h"
#include "etl/timer.h"
#include "etl/timer_node.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

#if defined(ETL_COMPILER_MICROSOFT)
  #include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  uint64_t ticks = 0ULL;

  //***************************************************************************
  /// A lock policy that counts the number of times it is locked and unlocked.
  /// The lock count should always be 0 or 1.
  //***************************************************************************
  class LockPolicy : public etl::timer::lock_policy
  {
  public:

    //*******************************************
    LockPolicy()
      : etl::timer::lock_policy()
      , count(0)
    {
    }

    //*******************************************
    ~LockPolicy()
    {
      assert(count == 0);
    }

    //*******************************************
    void lock() ETL_NOEXCEPT override
    {
      ++count;

      assert(count == 1);
    }

    //*******************************************
    void unlock() ETL_NOEXCEPT override
    {
      --count;

      assert(count == 0);
    }

    //*******************************************
    bool is_locked() ETL_NOEXCEPT override
    {
      return (count > 0);
    }

    //*******************************************
    int get_count() const
    {
      return count;
    }

  private:

    int count;
  };

  //*************************************************************************
  using CallbackDelegate = etl::delegate<void()>;
  using CallbackTimer    = etl::timer::callback_node<CallbackDelegate>;

  //*************************************************************************
  bool     global_callback_called = false;
  uint64_t global_call_count      = 0;

  static void global_callback()
  {
    global_callback_called = true;
    ++global_call_count;
  }

  void global_callback_clear()
  {
    global_callback_called = false;
    global_call_count      = 0;
  }

  //*************************************************************************
  class Object
  {
  public:

    Object()
      : member_callback_called(false)
      , member_call_count(0)
    {
    }

    void member()
    {
      member_callback_called = true;
      ++member_call_count;
    }

    void clear()
    {
      member_callback_called = false;
      member_call_count      = 0;
    }

    bool     member_callback_called;
    uint64_t member_call_count;
  };

  static Object object;

  //*************************************************************************
  struct CallbackMessage : public etl::message<1>
  {
    CallbackMessage(int value_)
      : value(value_)
      , config(ETL_NULLPTR)
    {
    }

    int                       value;
    const etl::timer::config* config;
  };

  class MessageProcessor : public etl::message_router<MessageProcessor, CallbackMessage>
  {
  public:

    MessageProcessor()
      : etl::message_router<MessageProcessor, CallbackMessage>()
      , message_callback_called(false)
      , message_value(0)
      , message_call_count(0)
    {
    }

    //*******************************************
    void on_receive(const CallbackMessage& msg)
    {
      message_callback_called = true;
      message_value           = msg.value;
      ++message_call_count;
    }

    //*******************************************
    void on_receive_unknown(const etl::imessage&) {}

    //*******************************************
    void clear()
    {
      message_callback_called = false;
      message_value           = 0;
      message_call_count      = 0;
    }

    bool     message_callback_called;
    int      message_value;
    uint64_t message_call_count;
  };

  using MessageTimer = etl::timer::message_node<CallbackMessage>;

  MessageProcessor message_processor;
  CallbackMessage  callback_message(42);

  //*************************************************************************
  // Notification of the time to the next timer that is due to expire.
  //*************************************************************************
  std::vector<uint32_t> time_to_next;

  //***********************************
  /// A timer_time_to_next_notification that calls next_timer_notification().
  /// Passed to the intrusive timer to notify the user of the time to the next timer that is due to expire.
  //***********************************
  class time_to_next_notification : public etl::timer::time_to_next_notification
  {
  public:

    void invoke(uint32_t delta) override
    {
      time_to_next.push_back(delta);
    }
  };

  time_to_next_notification notification;

  SUITE(test_intrusive_timer)
  {
    //*************************************************************************
    TEST(test_intrusive_timer_construction)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      CHECK_FALSE(timer_controller.is_running());
      CHECK_EQUAL(0U, timer_controller.number_of_timers());
      CHECK_EQUAL(0U, lock_policy.get_count());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_register_timers)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Single_Shot));
      etl::timer::node node2(etl::timer::config(200, etl::timer::mode::Repeating));
      etl::timer::node node3(etl::timer::config(300, etl::timer::mode::Repeating));

      CHECK_FALSE(timer_controller.is_registered(node1));
      CHECK_FALSE(timer_controller.is_registered(node2));
      CHECK_FALSE(timer_controller.is_registered(node3));

      CHECK_TRUE(timer_controller.register_timer(node1));
      CHECK_TRUE(timer_controller.is_registered(node1));
      CHECK_EQUAL(1U, timer_controller.number_of_timers());

      CHECK_TRUE(timer_controller.register_timer(node2));
      CHECK_TRUE(timer_controller.is_registered(node2));
      CHECK_EQUAL(2U, timer_controller.number_of_timers());

      CHECK_TRUE(timer_controller.register_timer(node3));
      CHECK_TRUE(timer_controller.is_registered(node3));
      CHECK_EQUAL(3U, timer_controller.number_of_timers());

      CHECK_FALSE(timer_controller.is_running());
      CHECK_EQUAL(0U, lock_policy.get_count());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_register_timers_more_than_once)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller1(lock_policy);
      etl::intrusive_timer timer_controller2(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Single_Shot));

      bool success = false;

      // Register the timer with the first controller twice.
      success = timer_controller1.register_timer(node1);
      CHECK_TRUE(success);
      success = timer_controller1.register_timer(node1);
      CHECK_FALSE(success);
      CHECK_EQUAL(1U, timer_controller1.number_of_timers());

      // Register the same timer with the second controller.
      success = timer_controller2.register_timer(node1);
      CHECK_FALSE(success);
      CHECK_EQUAL(0U, timer_controller2.number_of_timers());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_register_timers_to_one_then_unregister_then_register_with_another)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller1(lock_policy);
      etl::intrusive_timer timer_controller2(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Single_Shot));

      bool success = false;

      // Register the timer with the first controller.
      success = timer_controller1.register_timer(node1);
      CHECK_TRUE(success);

      // Unregister the timer from the first controller.
      success = timer_controller1.unregister_timer(node1);
      CHECK_TRUE(success);

      // Register the same timer with the second controller.
      success = timer_controller2.register_timer(node1);
      CHECK_TRUE(success);
    }

    //*************************************************************************
    TEST(test_intrusive_timer_start_stop_timers)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Single_Shot));

      // Start without registering.
      CHECK_FALSE(timer_controller.start(node1));
      CHECK_FALSE(timer_controller.has_active_timer());

      // Stop without registering.
      CHECK_FALSE(timer_controller.stop(node1));
      CHECK_FALSE(timer_controller.has_active_timer());

      // Register node1.
      CHECK_TRUE(timer_controller.register_timer(node1));

      // Start.
      CHECK_TRUE(timer_controller.start(node1));
      CHECK_TRUE(timer_controller.has_active_timer());

      // Stop.
      CHECK_TRUE(timer_controller.stop(node1));
      CHECK_FALSE(timer_controller.has_active_timer());

      // Start twice.
      CHECK_TRUE(timer_controller.start(node1));
      CHECK_TRUE(timer_controller.has_active_timer());
      CHECK_FALSE(timer_controller.start(node1));
      CHECK_TRUE(timer_controller.has_active_timer());

      // Stop twice.
      CHECK_TRUE(timer_controller.stop(node1));
      CHECK_FALSE(timer_controller.has_active_timer());
      CHECK_FALSE(timer_controller.stop(node1));
      CHECK_FALSE(timer_controller.has_active_timer());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_find_timers)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Repeating));
      etl::timer::node node2(etl::timer::config(200, etl::timer::mode::Repeating));
      etl::timer::node node3(etl::timer::config(300, etl::timer::mode::Repeating));
      etl::timer::node node4(etl::timer::config(400, etl::timer::mode::Repeating));

      timer_controller.register_timer(node1);
      timer_controller.register_timer(node2);
      timer_controller.register_timer(node3);

      // Check the ids are valid, except for the unregistered node4.
      CHECK_FALSE(node1.get_id() == etl::timer::id::No_Timer);
      CHECK_FALSE(node2.get_id() == etl::timer::id::No_Timer);
      CHECK_FALSE(node3.get_id() == etl::timer::id::No_Timer);
      CHECK_TRUE(node4.get_id() == etl::timer::id::No_Timer);

      // Check we can find the timers by their id.
      CHECK_EQUAL(&node1, timer_controller.find_timer(node1.get_id()));
      CHECK_EQUAL(&node2, timer_controller.find_timer(node2.get_id()));
      CHECK_EQUAL(&node3, timer_controller.find_timer(node3.get_id()));
      CHECK_EQUAL(etl::timer::node::end(), timer_controller.find_timer(node4.get_id()));
    }

    //*************************************************************************
    TEST(test_intrusive_timer_unregister_timers)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Single_Shot));
      etl::timer::node node2(etl::timer::config(200, etl::timer::mode::Repeating));
      etl::timer::node node3(etl::timer::config(300, etl::timer::mode::Repeating));

      timer_controller.register_timer(node1);
      timer_controller.register_timer(node2);
      timer_controller.register_timer(node3);

      CHECK_EQUAL(3U, timer_controller.number_of_timers());
      CHECK_TRUE(timer_controller.unregister_timer(node1));
      CHECK_EQUAL(2U, timer_controller.number_of_timers());
      CHECK_TRUE(timer_controller.unregister_timer(node2));
      CHECK_EQUAL(1U, timer_controller.number_of_timers());
      CHECK_TRUE(timer_controller.unregister_timer(node3));
      CHECK_EQUAL(0U, timer_controller.number_of_timers());
      CHECK_FALSE(timer_controller.is_running());
      CHECK_EQUAL(0U, lock_policy.get_count());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_register_timers_then_modify_them_individually)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Single_Shot));
      etl::timer::node node2(etl::timer::config(200, etl::timer::mode::Repeating));
      etl::timer::node node3(etl::timer::config(300, etl::timer::mode::Repeating));

      timer_controller.register_timer(node1);
      timer_controller.register_timer(node2);
      timer_controller.register_timer(node3);

      // Modify the timer intervals.
      timer_controller.set_interval(node1, 150U);
      timer_controller.set_interval(node2, 250U);
      timer_controller.set_interval(node3, 350U);

      // Modify the timer modes.
      timer_controller.set_mode(node1, etl::timer::mode::Repeating);
      timer_controller.set_mode(node2, etl::timer::mode::Single_Shot);
      timer_controller.set_mode(node3, etl::timer::mode::Single_Shot);

      CHECK_EQUAL(150U, node1.get_interval());
      CHECK_EQUAL(250U, node2.get_interval());
      CHECK_EQUAL(350U, node3.get_interval());

      CHECK_EQUAL(etl::timer::mode::Repeating, node1.get_mode());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, node2.get_mode());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, node3.get_mode());

      CHECK_EQUAL(0U, lock_policy.get_count());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_register_timers_then_modify_them_with_config)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::node node1(etl::timer::config(100, etl::timer::mode::Single_Shot));
      etl::timer::node node2(etl::timer::config(200, etl::timer::mode::Repeating));
      etl::timer::node node3(etl::timer::config(300, etl::timer::mode::Repeating));

      timer_controller.register_timer(node1);
      timer_controller.register_timer(node2);
      timer_controller.register_timer(node3);

      // Modify the timer configs.
      etl::timer::config config1(150, etl::timer::mode::Repeating);
      etl::timer::config config2(250, etl::timer::mode::Single_Shot);
      etl::timer::config config3(350, etl::timer::mode::Single_Shot);

      timer_controller.set_config(node1, config1);
      timer_controller.set_config(node2, config2);
      timer_controller.set_config(node3, config3);

      CHECK_EQUAL(150U, node1.get_interval());
      CHECK_EQUAL(250U, node2.get_interval());
      CHECK_EQUAL(350U, node3.get_interval());

      CHECK_EQUAL(etl::timer::mode::Repeating, node1.get_mode());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, node2.get_mode());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, node3.get_mode());

      CHECK_EQUAL(0U, lock_policy.get_count());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_single_shot)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification);
      time_to_next.clear();

      global_callback_clear();
      etl::timer::config config1(37, etl::timer::mode::Single_Shot);
      CallbackDelegate   callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer      callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer::config config2(23, etl::timer::mode::Single_Shot);
      CallbackDelegate   callback_delegate2 = CallbackDelegate::create<Object, &Object::member>(object);
      CallbackTimer      callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer::config config3(11, etl::timer::mode::Single_Shot);
      MessageTimer       callback_timer3(config3, message_processor, callback_message);

      timer_controller.register_timer(callback_timer1);
      timer_controller.register_timer(callback_timer2);
      timer_controller.register_timer(callback_timer3);

      timer_controller.start(callback_timer1); // 37, the first timer started is the first due to expire.
      timer_controller.start(callback_timer3); // 11, the second timer started is now first due to expire, 37 is second.
      timer_controller.start(callback_timer2); // 23, the third timer started is now second due to expire.37 is third.

      // Check the notifications.
      CHECK_EQUAL(37, time_to_next[0]); // 37, the first timer started is the first due to expire.
      CHECK_EQUAL(11, time_to_next[1]); // 11, the second timer started is now first due to expire.

      time_to_next.clear();

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1UL;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      CHECK_TRUE(global_callback_called);
      CHECK_EQUAL(1, global_call_count);

      CHECK_TRUE(message_processor.message_callback_called);
      CHECK_EQUAL(1, message_processor.message_call_count);

      // Check the notifications.
      CHECK_EQUAL(3, time_to_next.size());
      CHECK_EQUAL(12, time_to_next[0]);                                // 23, the next after 11 expired.
      CHECK_EQUAL(14, time_to_next[1]);                                // 37, the next after 23 expired.
      CHECK_EQUAL(etl::timer::interval::No_Interval, time_to_next[2]); // No more timers due.

      time_to_next.clear();
    }

    //*************************************************************************
    TEST(test_intrusive_timer_repeating)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification);
      time_to_next.clear();

      global_callback_clear();
      etl::timer::config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer      callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer::config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate2 = CallbackDelegate::create<Object, &Object::member>(object);
      CallbackTimer      callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer::config config3(11, etl::timer::mode::Repeating);
      MessageTimer       callback_timer3(config3, message_processor, callback_message);

      timer_controller.register_timer(callback_timer1);
      timer_controller.register_timer(callback_timer2);
      timer_controller.register_timer(callback_timer3);

      timer_controller.start(callback_timer1); // 37, the first timer started is the first due to expire.
      timer_controller.start(callback_timer3); // 11, the second timer started is now first due to expire, 37 is second.
      timer_controller.start(callback_timer2); // 23, the third timer started is now second due to expire.37 is third.

      // Check the notifications.
      CHECK_EQUAL(2, time_to_next.size());
      CHECK_EQUAL(37, time_to_next[0]); // 37, the first timer started is the first due to expire.
      CHECK_EQUAL(11, time_to_next[1]); // 11, the second timer started is now first due to expire.

      time_to_next.clear();

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1UL;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      // Check the callback counts.
      CHECK_TRUE(global_callback_called);
      CHECK_EQUAL(2, global_call_count);

      CHECK_TRUE(object.member_callback_called);
      CHECK_EQUAL(4, object.member_call_count);

      CHECK_TRUE(message_processor.message_callback_called);
      CHECK_EQUAL(9, message_processor.message_call_count);

      // Check the notifications.
      CHECK_EQUAL(15, time_to_next.size());
      CHECK_EQUAL(11, time_to_next[0]);
      CHECK_EQUAL(1, time_to_next[1]);
      CHECK_EQUAL(10, time_to_next[2]);
      CHECK_EQUAL(4, time_to_next[3]);
      CHECK_EQUAL(7, time_to_next[4]);
      CHECK_EQUAL(2, time_to_next[5]);
      CHECK_EQUAL(9, time_to_next[6]);
      CHECK_EQUAL(11, time_to_next[7]);
      CHECK_EQUAL(3, time_to_next[8]);
      CHECK_EQUAL(5, time_to_next[9]);
      CHECK_EQUAL(3, time_to_next[10]);
      CHECK_EQUAL(11, time_to_next[11]);
      CHECK_EQUAL(4, time_to_next[12]);
      CHECK_EQUAL(7, time_to_next[13]);
      CHECK_EQUAL(11, time_to_next[14]);

      time_to_next.clear();
    }

    //*************************************************************************
    TEST(test_intrusive_timer_repeating_bigger_step)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification);
      time_to_next.clear();

      global_callback_clear();
      etl::timer::config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer      callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer::config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate2 = CallbackDelegate::create<Object, &Object::member>(object);
      CallbackTimer      callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer::config config3(11, etl::timer::mode::Repeating);
      MessageTimer       callback_timer3(config3, message_processor, callback_message);

      timer_controller.register_timer(callback_timer1);
      timer_controller.register_timer(callback_timer2);
      timer_controller.register_timer(callback_timer3);

      timer_controller.start(callback_timer1); // 37, the first timer started is the first due to expire.
      timer_controller.start(callback_timer3); // 11, the second timer started is now first due to expire, 37 is second.
      timer_controller.start(callback_timer2); // 23, the third timer started is now second due to expire.37 is third.

      // Check the notifications.
      CHECK_EQUAL(2, time_to_next.size());
      CHECK_EQUAL(37, time_to_next[0]); // 37, the first timer started is the first due to expire.
      CHECK_EQUAL(11, time_to_next[1]); // 11, the second timer started is now first due to expire.

      time_to_next.clear();

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 3UL;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      // Check the callback counts.
      CHECK_TRUE(global_callback_called);
      CHECK_EQUAL(2, global_call_count);

      CHECK_TRUE(object.member_callback_called);
      CHECK_EQUAL(4, object.member_call_count);

      CHECK_TRUE(message_processor.message_callback_called);
      CHECK_EQUAL(9, message_processor.message_call_count);

      // Check the notifications.
      CHECK_EQUAL(14, time_to_next.size());
      CHECK_EQUAL(10, time_to_next[0]);
      CHECK_EQUAL(9, time_to_next[1]);
      CHECK_EQUAL(4, time_to_next[2]);
      CHECK_EQUAL(5, time_to_next[3]);
      CHECK_EQUAL(1, time_to_next[4]);
      CHECK_EQUAL(7, time_to_next[5]);
      CHECK_EQUAL(9, time_to_next[6]);
      CHECK_EQUAL(3, time_to_next[7]);
      CHECK_EQUAL(5, time_to_next[8]);
      CHECK_EQUAL(2, time_to_next[9]);
      CHECK_EQUAL(10, time_to_next[10]);
      CHECK_EQUAL(2, time_to_next[11]);
      CHECK_EQUAL(6, time_to_next[12]);
      CHECK_EQUAL(11, time_to_next[13]);

      time_to_next.clear();
    }

    //*************************************************************************
    TEST(test_intrusive_timer_repeating_register_unregister)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification);
      time_to_next.clear();

      global_callback_clear();
      etl::timer::config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer      callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer::config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate2 = CallbackDelegate::create<Object, &Object::member>(object);
      CallbackTimer      callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer::config config3(11, etl::timer::mode::Repeating);
      MessageTimer       callback_timer3(config3, message_processor, callback_message);

      timer_controller.register_timer(callback_timer2);
      timer_controller.register_timer(callback_timer3);

      timer_controller.start(callback_timer3);
      timer_controller.start(callback_timer2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        if (ticks == 36)
        {
          timer_controller.unregister_timer(callback_timer2);

          timer_controller.register_timer(callback_timer1);
          timer_controller.start(callback_timer1);
        }

        ticks += step;
        timer_controller.tick(step);
      }

      // Check the callback counts.
      CHECK_TRUE(global_callback_called);
      CHECK_EQUAL(1, global_call_count);

      CHECK_TRUE(object.member_callback_called);
      CHECK_EQUAL(1, object.member_call_count);

      CHECK_TRUE(message_processor.message_callback_called);
      CHECK_EQUAL(9, message_processor.message_call_count);

      // Check the notifications.
      CHECK_EQUAL(12, time_to_next.size());
      CHECK_EQUAL(11, time_to_next[0]);  // Start to timer3
      CHECK_EQUAL(11, time_to_next[1]);  // to timer3
      CHECK_EQUAL(1, time_to_next[2]);   // to timer2
      CHECK_EQUAL(10, time_to_next[3]);  // to timer3
      CHECK_EQUAL(11, time_to_next[4]);  // to timer3
      CHECK_EQUAL(11, time_to_next[5]);  // to timer3
      CHECK_EQUAL(11, time_to_next[6]);  // to timer3
      CHECK_EQUAL(7, time_to_next[7]);   // to timer1
      CHECK_EQUAL(4, time_to_next[8]);   // to timer3
      CHECK_EQUAL(11, time_to_next[9]);  // to timer3
      CHECK_EQUAL(11, time_to_next[10]); // to timer3
      CHECK_EQUAL(11, time_to_next[11]); // to timer3

      time_to_next.clear();
    }

    //*************************************************************************
    TEST(test_intrusive_timer_repeating_clear)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification);
      time_to_next.clear();

      global_callback_clear();
      etl::timer::config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer      callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer::config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate2 = CallbackDelegate::create<Object, &Object::member>(object);
      CallbackTimer      callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer::config config3(11, etl::timer::mode::Repeating);
      MessageTimer       callback_timer3(config3, message_processor, callback_message);

      timer_controller.register_timer(callback_timer1);
      timer_controller.register_timer(callback_timer2);
      timer_controller.register_timer(callback_timer3);

      timer_controller.start(callback_timer1); // 37, the first timer started is the first due to expire.
      timer_controller.start(callback_timer3); // 11, the second timer started is now first due to expire, 37 is second.
      timer_controller.start(callback_timer2); // 23, the third timer started is now second due to expire.37 is third.

      time_to_next.clear();

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1UL;

      while (ticks <= 100U)
      {
        ticks += step;

        if (ticks == 40)
        {
          timer_controller.clear();
        }

        timer_controller.tick(step);
      }

      // Check that the timers have been cleared.
      CHECK_EQUAL(3, timer_controller.number_of_timers());
      CHECK_TRUE(callback_timer1.get_id() == etl::timer::id::No_Timer);
      CHECK_TRUE(callback_timer2.get_id() == etl::timer::id::No_Timer);
      CHECK_TRUE(callback_timer3.get_id() == etl::timer::id::No_Timer);
      CHECK_TRUE(callback_timer1.get_interval() == etl::timer::interval::No_Interval);
      CHECK_TRUE(callback_timer2.get_interval() == etl::timer::interval::No_Interval);
      CHECK_TRUE(callback_timer3.get_interval() == etl::timer::interval::No_Interval);
      CHECK_TRUE(callback_timer1.get_mode() == etl::timer::mode::No_Mode);
      CHECK_TRUE(callback_timer2.get_mode() == etl::timer::mode::No_Mode);
      CHECK_TRUE(callback_timer3.get_mode() == etl::timer::mode::No_Mode);

      // Check the callback counts.
      CHECK_TRUE(global_callback_called);
      CHECK_EQUAL(1, global_call_count);

      CHECK_TRUE(object.member_callback_called);
      CHECK_EQUAL(1, object.member_call_count);

      CHECK_TRUE(message_processor.message_callback_called);
      CHECK_EQUAL(3, message_processor.message_call_count);

      // Check the notifications.
      CHECK_EQUAL(5, time_to_next.size());
      CHECK_EQUAL(11, time_to_next[0]);
      CHECK_EQUAL(1, time_to_next[1]);
      CHECK_EQUAL(10, time_to_next[2]);
      CHECK_EQUAL(4, time_to_next[3]);
      CHECK_EQUAL(7, time_to_next[4]);

      time_to_next.clear();
    }

    //*************************************************************************
    TEST(test_intrusive_timer_delayed_immediate)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification);
      time_to_next.clear();

      global_callback_clear();
      etl::timer::config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer      callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer::config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate   callback_delegate2 = CallbackDelegate::create<Object, &Object::member>(object);
      CallbackTimer      callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer::config config3(11, etl::timer::mode::Repeating);
      MessageTimer       callback_timer3(config3, message_processor, callback_message);

      timer_controller.register_timer(callback_timer1);
      timer_controller.register_timer(callback_timer2);
      timer_controller.register_timer(callback_timer3);

      time_to_next.clear();

      timer_controller.enable(true);

      ticks = 0;
      timer_controller.tick(uint32_t(ticks));

      timer_controller.start(callback_timer1, etl::timer::start::Immediate);
      timer_controller.start(callback_timer2, etl::timer::start::Immediate);
      timer_controller.start(callback_timer3, etl::timer::start::Delayed);

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      // Check the callback counts.
      CHECK_TRUE(global_callback_called);
      CHECK_EQUAL(3, global_call_count);

      CHECK_TRUE(object.member_callback_called);
      CHECK_EQUAL(5, object.member_call_count);

      CHECK_TRUE(message_processor.message_callback_called);
      CHECK_EQUAL(9, message_processor.message_call_count);

      // Check the notifications.
      CHECK_EQUAL(18, time_to_next.size());
      CHECK_EQUAL(37, time_to_next[0]);
      CHECK_EQUAL(23, time_to_next[1]);
      CHECK_EQUAL(11, time_to_next[2]);
      CHECK_EQUAL(11, time_to_next[3]);
      CHECK_EQUAL(1, time_to_next[4]);
      CHECK_EQUAL(10, time_to_next[5]);
      CHECK_EQUAL(4, time_to_next[6]);
      CHECK_EQUAL(7, time_to_next[7]);
      CHECK_EQUAL(2, time_to_next[8]);
      CHECK_EQUAL(9, time_to_next[9]);
      CHECK_EQUAL(11, time_to_next[10]);
      CHECK_EQUAL(3, time_to_next[11]);
      CHECK_EQUAL(5, time_to_next[12]);
      CHECK_EQUAL(3, time_to_next[13]);
      CHECK_EQUAL(11, time_to_next[14]);
      CHECK_EQUAL(4, time_to_next[15]);
      CHECK_EQUAL(7, time_to_next[16]);
      CHECK_EQUAL(11, time_to_next[17]);

      time_to_next.clear();
    }

    //*************************************************************************
    struct FunctorExternalCount
    {
      FunctorExternalCount(int& call_count_)
        : call_count(&call_count_)
      {
      }

      void operator()()
      {
        ++(*call_count);
      }

      int* call_count;
    };

    TEST(test_intrusive_timer_with_functor_with_pointer_to_count)
    {
      int call_count = 0;

      FunctorExternalCount timer_functor(call_count);

      LockPolicy           lock_policy;
      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::callback_node<FunctorExternalCount> node1(etl::timer::config(1, etl::timer::mode::Single_Shot), timer_functor);

      timer_controller.register_timer(node1);
      timer_controller.enable(true);
      timer_controller.start(node1);
      timer_controller.tick(1);

      CHECK_EQUAL(1, call_count);
    }

    //*************************************************************************
    struct FunctorInternalCount
    {
      FunctorInternalCount()
        : call_count(0)
      {
      }

      void operator()()
      {
        ++call_count;
      }

      int call_count;
    };

    TEST(test_intrusive_timer_with_functor_reference)
    {
      FunctorInternalCount timer_functor;

      LockPolicy           lock_policy;
      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::callback_ref_node<FunctorInternalCount> node1(etl::timer::config(1, etl::timer::mode::Single_Shot), timer_functor);

      timer_controller.register_timer(node1);
      timer_controller.enable(true);
      timer_controller.start(node1);
      timer_controller.tick(1);

      CHECK_EQUAL(1, timer_functor.call_count);
    }

    //*************************************************************************
    TEST(test_intrusive_timer_with_function_pointer)
    {
      global_callback_clear();

      LockPolicy           lock_policy;
      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::callback_ref_node<decltype(global_callback)> node1(etl::timer::config(1, etl::timer::mode::Single_Shot), global_callback);

      timer_controller.register_timer(node1);
      timer_controller.enable(true);
      timer_controller.start(node1);
      timer_controller.tick(1);

      CHECK_EQUAL(1, global_call_count);
    }

    //*************************************************************************
    TEST(test_intrusive_timer_with_lambda)
    {
      int call_count = 0;

      auto timer_lambda = [&call_count]()
      {
        ++call_count;
      };

      LockPolicy           lock_policy;
      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer::callback_node<decltype(timer_lambda)> node1(etl::timer::config(1, etl::timer::mode::Single_Shot), timer_lambda);

      timer_controller.register_timer(node1);
      timer_controller.enable(true);
      timer_controller.start(node1);
      timer_controller.tick(1);

      CHECK_EQUAL(1, call_count);
    }

    //*************************************************************************
    struct Object
    {
      Object()
        : call_count(0)
      {
      }

      void Increment()
      {
        ++call_count;
      }

      int call_count;
    };

    static Object object;

    TEST(test_intrusive_timer_with_delegate)
    {
      auto               delegate = etl::delegate<void()>::create<Object, &Object::Increment>(object);
      etl::timer::config config(etl::timer::config(1, etl::timer::mode::Single_Shot));

      etl::timer::callback_node<decltype(delegate)> node(config, delegate);

      LockPolicy           lock_policy;
      etl::intrusive_timer timer_controller(lock_policy);

      timer_controller.register_timer(node);
      timer_controller.enable(true);
      timer_controller.start(node);
      timer_controller.tick(1);

      CHECK_EQUAL(1, object.call_count);
    }
    //*************************************************************************
#if REALTIME_TEST

  #if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently
                                     // supported
    #define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
    #define FIX_PROCESSOR_AFFINITY SetThreadAffinityMask(GetCurrentThread(), 1);
  #else
    #define RAISE_THREAD_PRIORITY
    #define FIX_PROCESSOR_AFFINITY
  #endif

    //*************************************************************************
    class semaphore_lock_policy : public etl::timer::lock_policy
    {
    public:

      //*******************************************
      semaphore_lock_policy()
        : semaphore(0)
      {
      }

      //*******************************************
      void lock() ETL_NOEXCEPT override
      {
        ++semaphore;

        // Add a small delay to cause the locks to more likely contend.
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      //*******************************************
      void unlock() ETL_NOEXCEPT override
      {
        --semaphore;
      }

      //*******************************************
      bool is_locked() ETL_NOEXCEPT override
      {
        return (semaphore > 0);
      }

      std::atomic<int> semaphore;
    };

    semaphore_lock_policy real_time_lock_policy;
    etl::intrusive_timer  timer_controller(real_time_lock_policy);

    std::atomic<uint64_t> real_time_ticks(0U);

    static void real_time_callback1();
    static void real_time_callback2();
    static void real_time_callback3();

    etl::timer::config config1(10, etl::timer::mode::Repeating);
    CallbackDelegate   callback_delegate1 = CallbackDelegate::create(real_time_callback1);
    CallbackTimer      callback_timer1(config1, callback_delegate1);

    etl::timer::config config2(10, etl::timer::mode::Repeating);
    CallbackDelegate   callback_delegate2 = CallbackDelegate::create(real_time_callback2);
    CallbackTimer      callback_timer2(config2, callback_delegate2);

    etl::timer::config config3(10, etl::timer::mode::Repeating);
    CallbackDelegate   callback_delegate3 = CallbackDelegate::create(real_time_callback3);
    CallbackTimer      callback_timer3(config3, callback_delegate3);

    int real_time_callback_count1 = 0;
    int real_time_callback_count2 = 0;
    int real_time_callback_count3 = 0;

    //*********************************
    static void real_time_callback1()
    {
      ++real_time_callback_count1;
    }

    static void real_time_callback2()
    {
      ++real_time_callback_count2;
    }

    static void real_time_callback3()
    {
      ++real_time_callback_count3;
    }

    //*********************************
    void timer_event()
    {
      uint32_t delta = 1;

      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY;

      while (real_time_ticks.load() <= 1000U)
      {
        if (timer_controller.tick(delta))
        {
          delta = 1;
        }
        else
        {
          // Enable the assert to check that the tick can sometimes fail to process due to contending locks.
          // This is expected behaviour.
          // assert(false);
          ++delta;
        }

        ++real_time_ticks;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }

    //*********************************
    TEST(test_intrusive_timer_threads)
    {
      FIX_PROCESSOR_AFFINITY;

      timer_controller.register_timer(callback_timer1);
      timer_controller.register_timer(callback_timer2);
      timer_controller.register_timer(callback_timer3);

      timer_controller.start(callback_timer1);
      timer_controller.start(callback_timer2);
      timer_controller.start(callback_timer3);

      timer_controller.enable(true);

      std::thread t1(timer_event);

      int count1 = 0;
      int count2 = 0;
      int count3 = 0;

      while (real_time_ticks.load() <= 1000U)
      {
        if (count1 == 50)
        {
          timer_controller.stop(callback_timer1);
        }

        if (count1 == 100)
        {
          timer_controller.start(callback_timer1);
          count1 = 0;
        }

        if (count2 == 60)
        {
          timer_controller.stop(callback_timer2);
        }

        if (count2 == 110)
        {
          timer_controller.start(callback_timer2);
          count2 = 0;
        }

        if (count3 == 70)
        {
          timer_controller.stop(callback_timer3);
        }

        if (count3 == 120)
        {
          timer_controller.start(callback_timer3);
          count3 = 0;
        }

        ++count1;
        ++count2;
        ++count3;

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      // Join the thread with the main thread
      t1.join();

      CHECK_TRUE(real_time_callback_count1 > 30);
      CHECK_TRUE(real_time_callback_count2 > 30);
      CHECK_TRUE(real_time_callback_count3 > 30);
    }
#endif
  }
} // namespace
