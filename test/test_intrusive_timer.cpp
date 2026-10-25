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

#include "etl/error_handler.h"
#include "etl/intrusive_timer.h"
#include "etl/timer.h"
#include "etl/timer_node.h"

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

  class TimerInsertRemoveTest
  {
  public:

    uint32_t inserted;
    uint32_t removed;
    TimerInsertRemoveTest()
      : inserted(0)
      , removed(0)
    {
    }

    void insert_handler(etl::timer::id::type id_)
    {
      (void)id_;
      inserted++;
    }

    void remove_handler(etl::timer::id::type id_)
    {
      (void)id_;
      removed++;
    }

    void clear(void)
    {
      inserted = 0;
      removed  = 0;
    }
  };

  TimerInsertRemoveTest timerInsertRemoveTest;

  //***************************************************************************
  /// A lock policy that counts the number of times it is locked and unlocked.
  /// The lock count should always be 0 or 1.
  //***************************************************************************
  class LockPolicy : public etl::timer_lock_policy
  {
  public:

    //*******************************************
    LockPolicy()
      : timer_lock_policy()
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
  using CallbackTimer    = etl::timer_callback_node<CallbackDelegate>;

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

    int                      value;
    const etl::timer_config* config;
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
    void on_receive_unknown(const etl::imessage& msg) {}

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

  using MessageTimer = etl::timer_message_node<CallbackMessage>;

  MessageProcessor message_processor;
  CallbackMessage  callback_message(42);

  //*************************************************************************
  // Notification of the time to the next timer that is due to expire.
  //*************************************************************************
  std::vector<uint32_t> time_to_next;

  //***********************************
  /// Notification of the time to the next timer, from the intrusive timer.
  //***********************************
  void next_timer_notification(uint32_t delta)
  {
    time_to_next.push_back(delta);
  }

  using notification_type = etl::intrusive_timer::time_to_next_notification_type;

  notification_type notification_delegate = notification_type::create(next_timer_notification);

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

      etl::timer_node node1(etl::timer_config(100, etl::timer::mode::Single_Shot));
      etl::timer_node node2(etl::timer_config(200, etl::timer::mode::Repeating));
      etl::timer_node node3(etl::timer_config(300, etl::timer::mode::Repeating));

      CHECK_TRUE(timer_controller.register_timer(node1));
      CHECK_EQUAL(1U, timer_controller.number_of_timers());
      CHECK_TRUE(timer_controller.register_timer(node2));
      CHECK_EQUAL(2U, timer_controller.number_of_timers());
      CHECK_TRUE(timer_controller.register_timer(node3));
      CHECK_EQUAL(3U, timer_controller.number_of_timers());
      CHECK_FALSE(timer_controller.is_running());
      CHECK_EQUAL(0U, lock_policy.get_count());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_double_register_timers)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller1(lock_policy);
      etl::intrusive_timer timer_controller2(lock_policy);

      etl::timer_node node1(etl::timer_config(100, etl::timer::mode::Single_Shot));

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
    TEST(test_intrusive_timer_find_timers)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer_node node1(etl::timer_config(100, etl::timer::mode::Repeating));
      etl::timer_node node2(etl::timer_config(200, etl::timer::mode::Repeating));
      etl::timer_node node3(etl::timer_config(300, etl::timer::mode::Repeating));
      etl::timer_node node4(etl::timer_config(400, etl::timer::mode::Repeating));

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
      CHECK_EQUAL(etl::timer_node::end(), timer_controller.find_timer(node4.get_id()));
    }

    //*************************************************************************
    TEST(test_intrusive_timer_unregister_timers)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer_node node1(etl::timer_config(100, etl::timer::mode::Single_Shot));
      etl::timer_node node2(etl::timer_config(200, etl::timer::mode::Repeating));
      etl::timer_node node3(etl::timer_config(300, etl::timer::mode::Repeating));

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
    TEST(test_intrusive_timer_register_timers_then_modify_them)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);

      etl::timer_node node1(etl::timer_config(100, etl::timer::mode::Single_Shot));
      etl::timer_node node2(etl::timer_config(200, etl::timer::mode::Repeating));
      etl::timer_node node3(etl::timer_config(300, etl::timer::mode::Repeating));

      timer_controller.register_timer(node1);
      timer_controller.register_timer(node2);
      timer_controller.register_timer(node3);

      // Modify the timer periods.
      timer_controller.set_period(node1, 150U);
      timer_controller.set_period(node2, 250U);
      timer_controller.set_period(node3, 350U);

      // Modify the timer modes.
      timer_controller.set_mode(node1, etl::timer::mode::Repeating);
      timer_controller.set_mode(node2, etl::timer::mode::Single_Shot);
      timer_controller.set_mode(node3, etl::timer::mode::Single_Shot);

      CHECK_EQUAL(150U, node1.get_period());
      CHECK_EQUAL(250U, node2.get_period());
      CHECK_EQUAL(350U, node3.get_period());

      CHECK_EQUAL(etl::timer::mode::Repeating, node1.get_mode());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, node2.get_mode());
      CHECK_EQUAL(etl::timer::mode::Single_Shot, node3.get_mode());

      CHECK_EQUAL(0U, lock_policy.get_count());
    }

    //*************************************************************************
    TEST(test_intrusive_timer_one_shot)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification_delegate);
      time_to_next.clear();

      global_callback_clear();
      etl::timer_config config1(37, etl::timer::mode::Single_Shot);
      CallbackDelegate  callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer     callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer_config config2(23, etl::timer::mode::Single_Shot);
      CallbackDelegate  callback_delegate2 = CallbackDelegate::create<Object, &Object::member, object>();
      CallbackTimer     callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer_config config3(11, etl::timer::mode::Single_Shot);
      MessageTimer      callback_timer3(config3, message_processor, callback_message);

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
      CHECK_EQUAL(12, time_to_next[0]); // 23, the next after 11 expired.
      CHECK_EQUAL(14, time_to_next[1]); // 37, the next after 23 expired.

      time_to_next.clear();
    }

    //*************************************************************************
    TEST(test_intrusive_timer_repeating)
    {
      LockPolicy lock_policy;

      etl::intrusive_timer timer_controller(lock_policy);
      timer_controller.set_next_timer_notification(notification_delegate);
      time_to_next.clear();

      global_callback_clear();
      etl::timer_config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate  callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer     callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer_config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate  callback_delegate2 = CallbackDelegate::create<Object, &Object::member, object>();
      CallbackTimer     callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer_config config3(11, etl::timer::mode::Repeating);
      MessageTimer      callback_timer3(config3, message_processor, callback_message);

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

      // Check the callback counts.
      CHECK_TRUE(global_callback_called);
      CHECK_EQUAL(2, global_call_count);

      CHECK_TRUE(object.member_callback_called);
      CHECK_EQUAL(4, object.member_call_count);

      CHECK_TRUE(message_processor.message_callback_called);
      CHECK_EQUAL(9, message_processor.message_call_count);

      // Check the notifications.
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
      timer_controller.set_next_timer_notification(notification_delegate);
      time_to_next.clear();

      global_callback_clear();
      etl::timer_config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate  callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer     callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer_config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate  callback_delegate2 = CallbackDelegate::create<Object, &Object::member, object>();
      CallbackTimer     callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer_config config3(11, etl::timer::mode::Repeating);
      MessageTimer      callback_timer3(config3, message_processor, callback_message);

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
      timer_controller.set_next_timer_notification(notification_delegate);
      time_to_next.clear();

      global_callback_clear();
      etl::timer_config config1(37, etl::timer::mode::Repeating);
      CallbackDelegate  callback_delegate1 = CallbackDelegate::create(global_callback);
      CallbackTimer     callback_timer1(config1, callback_delegate1);

      object.clear();
      etl::timer_config config2(23, etl::timer::mode::Repeating);
      CallbackDelegate  callback_delegate2 = CallbackDelegate::create<Object, &Object::member, object>();
      CallbackTimer     callback_timer2(config2, callback_delegate2);

      message_processor.clear();
      etl::timer_config config3(11, etl::timer::mode::Repeating);
      MessageTimer      callback_timer3(config3, message_processor, callback_message);

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

    //    //*************************************************************************
    //    TEST(test_intrusive_timer_repeating_clear)
    //    {
    //      etl::intrusive_timer<3> timer_controller;
    //
    //      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Repeating);
    //      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback, 23, etl::timer::mode::Repeating);
    //      etl::timer::id::type id3 = timer_controller.register_timer(free_callback2, 11, etl::timer::mode::Repeating);
    //
    //      object.tick_list.clear();
    //      free_tick_list1.clear();
    //      free_tick_list2.clear();
    //
    //      timer_controller.start(id1);
    //      timer_controller.start(id3);
    //      timer_controller.start(id2);
    //
    //      timer_controller.enable(true);
    //
    //      ticks = 0;
    //
    //      const uint32_t step = 1U;
    //
    //      while (ticks <= 100U)
    //      {
    //        ticks += step;
    //
    //        if (ticks == 40)
    //        {
    //          timer_controller.clear();
    //        }
    //
    //        timer_controller.tick(step);
    //      }
    //
    //      std::vector<uint64_t> compare1 = {37};
    //      std::vector<uint64_t> compare2 = {23};
    //      std::vector<uint64_t> compare3 = {11, 22, 33};
    //
    //      CHECK(object.tick_list.size() != 0);
    //      CHECK(free_tick_list1.size() != 0);
    //      CHECK(free_tick_list2.size() != 0);
    //
    //      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(), compare1.size());
    //      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
    //      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());
    //    }
    //
    //    //*************************************************************************
    //    TEST(test_intrusive_timer_delayed_immediate)
    //    {
    //      etl::intrusive_timer<3> timer_controller;
    //
    //      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Repeating);
    //      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback, 23, etl::timer::mode::Repeating);
    //      etl::timer::id::type id3 = timer_controller.register_timer(free_callback2, 11, etl::timer::mode::Repeating);
    //
    //      object.tick_list.clear();
    //      free_tick_list1.clear();
    //      free_tick_list2.clear();
    //
    //      timer_controller.enable(true);
    //
    //      ticks = 5;
    //      timer_controller.tick(uint32_t(ticks));
    //
    //      timer_controller.start(id1, etl::timer::start::Immediate);
    //      timer_controller.start(id2, etl::timer::start::Immediate);
    //      timer_controller.start(id3, etl::timer::start::Delayed);
    //
    //      const uint32_t step = 1U;
    //
    //      while (ticks <= 100U)
    //      {
    //        ticks += step;
    //        timer_controller.tick(step);
    //      }
    //
    //      std::vector<uint64_t> compare1 = {6, 42, 79};
    //      std::vector<uint64_t> compare2 = {6, 28, 51, 74, 97};
    //      std::vector<uint64_t> compare3 = {16, 27, 38, 49, 60, 71, 82, 93};
    //
    //      CHECK(object.tick_list.size() != 0);
    //      CHECK(free_tick_list1.size() != 0);
    //      CHECK(free_tick_list2.size() != 0);
    //
    //      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(), compare1.size());
    //      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
    //      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());
    //    }
    //
    //    //*************************************************************************
    //    TEST(test_intrusive_timer_one_shot_big_step_short_delay_insert)
    //    {
    //      etl::intrusive_timer<3> timer_controller;
    //
    //      etl::timer::id::type id1 = timer_controller.register_timer(free_callback1, 15, etl::timer::mode::Single_Shot);
    //      etl::timer::id::type id2 = timer_controller.register_timer(free_callback2, 5, etl::timer::mode::Repeating);
    //
    //      free_tick_list1.clear();
    //      free_tick_list2.clear();
    //
    //      timer_controller.start(id1);
    //      timer_controller.start(id2);
    //
    //      timer_controller.enable(true);
    //
    //      ticks = 0;
    //
    //      const uint32_t step = 11U;
    //
    //      ticks += step;
    //      timer_controller.tick(step);
    //
    //      ticks += step;
    //      timer_controller.tick(step);
    //
    //      std::vector<uint64_t> compare1 = {22};
    //      std::vector<uint64_t> compare2 = {11, 11, 22, 22};
    //
    //      CHECK(free_tick_list1.size() != 0);
    //      CHECK(free_tick_list2.size() != 0);
    //
    //      CHECK_ARRAY_EQUAL(compare1.data(), free_tick_list1.data(), compare1.size());
    //      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list2.data(), compare2.size());
    //    }
    //
    //    //*************************************************************************
    //    TEST(test_intrusive_timer_one_shot_empty_list_huge_tick_before_insert)
    //    {
    //      etl::intrusive_timer<3> timer_controller;
    //
    //      etl::timer::id::type id1 = timer_controller.register_timer(free_callback1, 5, etl::timer::mode::Single_Shot);
    //
    //      free_tick_list1.clear();
    //
    //      timer_controller.start(id1);
    //
    //      timer_controller.enable(true);
    //
    //      ticks = 0;
    //
    //      const uint32_t step = 5U;
    //
    //      for (uint32_t i = 0U; i < step; ++i)
    //      {
    //        ++ticks;
    //        timer_controller.tick(1);
    //      }
    //
    //      // Huge tick count.
    //      timer_controller.tick(UINT32_MAX - step + 1);
    //
    //      timer_controller.start(id1);
    //
    //      for (uint32_t i = 0U; i < step; ++i)
    //      {
    //        ++ticks;
    //        timer_controller.tick(1);
    //      }
    //      std::vector<uint64_t> compare1 = {5, 10};
    //
    //      CHECK(free_tick_list1.size() != 0);
    //
    //      CHECK_ARRAY_EQUAL(compare1.data(), free_tick_list1.data(), compare1.size());
    //    }
    //
    //    //*************************************************************************
    //    TEST(message_timer_time_to_next_repeating)
    //    {
    //      etl::intrusive_timer<3> timer_controller;
    //
    //      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Repeating);
    //      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback, 23, etl::timer::mode::Repeating);
    //      etl::timer::id::type id3 = timer_controller.register_timer(free_callback2, 11, etl::timer::mode::Repeating);
    //
    //      timer_controller.start(id1);
    //      timer_controller.start(id3);
    //      timer_controller.start(id2);
    //
    //      timer_controller.enable(true);
    //
    //      CHECK_EQUAL(11, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(4, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(8, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(1, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(5, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(2, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(2, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(6, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(10, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(3, timer_controller.time_to_next());
    //
    //      timer_controller.tick(7);
    //      CHECK_EQUAL(4, timer_controller.time_to_next());
    //    }
    //
    //    //*************************************************************************
    //    TEST(message_timer_time_to_next_with_has_active_timer)
    //    {
    //      etl::intrusive_timer<3> timer_controller;
    //
    //      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Single_Shot);
    //      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback, 23, etl::timer::mode::Single_Shot);
    //      etl::timer::id::type id3 = timer_controller.register_timer(free_callback2, 11, etl::timer::mode::Single_Shot);
    //
    //      timer_controller.start(id1);
    //      timer_controller.start(id3);
    //      timer_controller.start(id2);
    //
    //      timer_controller.enable(true);
    //
    //      timer_controller.tick(11);
    //      CHECK_EQUAL(12, timer_controller.time_to_next());
    //      CHECK_TRUE(timer_controller.has_active_timer());
    //
    //      timer_controller.tick(23);
    //      CHECK_EQUAL(3, timer_controller.time_to_next());
    //      CHECK_TRUE(timer_controller.has_active_timer());
    //
    //      timer_controller.tick(2);
    //      CHECK_EQUAL(1, timer_controller.time_to_next());
    //      CHECK_TRUE(timer_controller.has_active_timer());
    //
    //      timer_controller.tick(1);
    //      CHECK_EQUAL(static_cast<etl::timer::interval::type>(etl::timer::interval::No_Active_Interval), timer_controller.time_to_next());
    //      CHECK_FALSE(timer_controller.has_active_timer());
    //    }
    //
    //    //*************************************************************************
    //    class test_object
    //    {
    //    public:
    //
    //      void call()
    //      {
    //        ++called;
    //      }
    //
    //      size_t called = 0UL;
    //    };
    //
    //    using callback_type = etl::iintrusive_timer::callback_type;
    //
    //    TEST(test_intrusive_timer_call_etl_delegate)
    //    {
    //      test_object            test_obj;
    //      callback_type          delegate_callback = callback_type::create<test_object, &test_object::call>(test_obj);
    //      etl::intrusive_timer<1> timer_controller;
    //
    //      timer_controller.enable(true);
    //
    //      etl::timer::id::type id = timer_controller.register_timer(delegate_callback, 5, etl::timer::mode::Single_Shot);
    //      timer_controller.start(id);
    //
    //      timer_controller.tick(4);
    //      CHECK(test_obj.called == 0);
    //
    //      timer_controller.tick(2);
    //      CHECK(test_obj.called == 1);
    //    }
    //
    //    //*************************************************************************
    //    TEST(test_intrusive_timer_is_active)
    //    {
    //      timerInsertRemoveTest.clear();
    //      etl::intrusive_timer<4> timer_controller;
    //
    //      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Single_Shot);
    //      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback, 23, etl::timer::mode::Single_Shot);
    //      etl::timer::id::type id3 = timer_controller.register_timer(free_callback2, 11, etl::timer::mode::Single_Shot);
    //
    //      timer_controller.set_insert_callback(
    //        event_callback_type::create<TimerInsertRemoveTest, timerInsertRemoveTest, &TimerInsertRemoveTest::insert_handler>());
    //      timer_controller.set_remove_callback(
    //        event_callback_type::create<TimerInsertRemoveTest, timerInsertRemoveTest, &TimerInsertRemoveTest::remove_handler>());
    //
    //      timer_controller.start(id1);
    //      timer_controller.start(id3);
    //      timer_controller.start(id2);
    //
    //      CHECK_EQUAL(3, timerInsertRemoveTest.inserted);
    //      CHECK_EQUAL(0, timerInsertRemoveTest.removed);
    //
    //      timer_controller.enable(true);
    //
    //      CHECK_TRUE(timer_controller.is_active(id1));
    //      CHECK_TRUE(timer_controller.is_active(id2));
    //      CHECK_TRUE(timer_controller.is_active(id3));
    //
    //      CHECK_EQUAL(3, timerInsertRemoveTest.inserted);
    //      CHECK_EQUAL(0, timerInsertRemoveTest.removed);
    //
    //      timer_controller.tick(11);
    //      CHECK_TRUE(timer_controller.is_active(id1));
    //      CHECK_TRUE(timer_controller.is_active(id2));
    //      CHECK_FALSE(timer_controller.is_active(id3));
    //
    //      CHECK_EQUAL(3, timerInsertRemoveTest.inserted);
    //      CHECK_EQUAL(1, timerInsertRemoveTest.removed);
    //
    //      timer_controller.tick(23 - 11);
    //      CHECK_TRUE(timer_controller.is_active(id1));
    //      CHECK_FALSE(timer_controller.is_active(id2));
    //      CHECK_FALSE(timer_controller.is_active(id3));
    //
    //      CHECK_EQUAL(3, timerInsertRemoveTest.inserted);
    //      CHECK_EQUAL(2, timerInsertRemoveTest.removed);
    //
    //      timer_controller.tick(37 - 23);
    //      CHECK_FALSE(timer_controller.is_active(id1));
    //      CHECK_FALSE(timer_controller.is_active(id2));
    //      CHECK_FALSE(timer_controller.is_active(id3));
    //
    //      CHECK_EQUAL(3, timerInsertRemoveTest.inserted);
    //      CHECK_EQUAL(3, timerInsertRemoveTest.removed);
    //    }
    //
    //    //*************************************************************************
    // #if REALTIME_TEST
    //
    //  #if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently
    //                                     // supported
    //    #define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
    //    #define FIX_PROCESSOR_AFFINITY SetThreadAffinityMask(GetCurrentThread(), 1);
    //  #else
    //    #define RAISE_THREAD_PRIORITY
    //    #define FIX_PROCESSOR_AFFINITY
    //  #endif
    //
    //    etl::intrusive_timer<3> controller;
    //
    //    void timer_event()
    //    {
    //      const uint32_t TICK = 1U;
    //      uint32_t       tick = TICK;
    //      ticks               = 1U;
    //
    //      RAISE_THREAD_PRIORITY;
    //      FIX_PROCESSOR_AFFINITY;
    //
    //      while (ticks <= 1000U)
    //      {
    //        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //
    //        if (controller.tick(tick))
    //        {
    //          tick = TICK;
    //        }
    //        else
    //        {
    //          tick += TICK;
    //        }
    //
    //        ++ticks;
    //      }
    //    }
    //
    //    TEST(test_intrusive_timer_threads)
    //    {
    //      FIX_PROCESSOR_AFFINITY;
    //
    //      etl::timer::id::type id1 = controller.register_timer(member_callback, 400, etl::timer::mode::Single_Shot);
    //      etl::timer::id::type id2 = controller.register_timer(free_function_callback, 100, etl::timer::mode::Repeating);
    //      etl::timer::id::type id3 = controller.register_timer(free_callback2, 10, etl::timer::mode::Repeating);
    //
    //      object.tick_list.clear();
    //      free_tick_list1.clear();
    //      free_tick_list2.clear();
    //
    //      controller.start(id1);
    //      controller.start(id2);
    //      // controller.start(id3);
    //
    //      controller.enable(true);
    //
    //      std::thread t1(timer_event);
    //
    //      bool restart_1 = true;
    //
    //      while (ticks <= 1000U)
    //      {
    //        if ((ticks > 200U) && (ticks < 500U))
    //        {
    //          controller.stop(id3);
    //        }
    //
    //        if ((ticks > 600U) && (ticks < 800U))
    //        {
    //          controller.start(id3);
    //        }
    //
    //        if ((ticks > 500U) && restart_1)
    //        {
    //          controller.start(id1);
    //          restart_1 = false;
    //        }
    //
    //        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    //      }
    //
    //      // Join the thread with the main thread
    //      t1.join();
    //
    //      CHECK_EQUAL(2U, object.tick_list.size());
    //      CHECK_EQUAL(10U, free_tick_list1.size());
    //      CHECK(free_tick_list2.size() < 65U);
    //
    //      // std::vector<uint64_t> compare1 = { 400, 900 };
    //      // std::vector<uint64_t> compare2 = { 100, 200, 300, 400, 500, 600, 700,
    //      // 800, 900, 1000 };
    //
    //      CHECK(object.tick_list.size() != 0);
    //      CHECK(free_tick_list1.size() != 0);
    //      CHECK(free_tick_list2.size() != 0);
    //
    //      // CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),
    //      // min(compare1.size(), object.tick_list.size()));
    //      // CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(),
    //      // min(compare2.size(), free_tick_list1.size()));
    //    }
    // #endif
  }
} // namespace
