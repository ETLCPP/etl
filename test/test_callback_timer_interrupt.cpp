/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2022 John Wellbelove

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

#include "etl/callback_timer_interrupt.h"
#include "etl/delegate.h"

#include <vector>

namespace
{
  uint64_t ticks = 0ULL;

  //***************************************************************************
  struct ScopedGuard
  {
    ScopedGuard()
    {
      ++guard_count;
    }

    ~ScopedGuard()
    {
      --guard_count;
    }

    static int guard_count;
  };

  int ScopedGuard::guard_count = 0;

  //***************************************************************************
  struct TimerLogEntry
  {
    etl::timer::id::type id;
    uint64_t time_called;
  };

  //***************************************************************************
  // Class callback via etl::delegate
  //***************************************************************************
  class Test
  {
  public:

    Test()
      : p_controller(nullptr)
    {
    }

    void callback()
    {
      tick_list.push_back(ticks);
    }

    void callback2()
    {
      tick_list.push_back(ticks);

      p_controller->start(2);
      p_controller->start(1);
    }

    void set_controller(etl::callback_timer_interrupt<3, ScopedGuard>& controller)
    {
      p_controller = &controller;
    }

    std::vector<uint64_t> tick_list;

    etl::callback_timer_interrupt<3, ScopedGuard>* p_controller;
  };

  using callback_type = etl::icallback_timer_interrupt<ScopedGuard>::callback_type;

  Test test;
  callback_type member_callback  = callback_type::create<Test, test, &Test::callback>();
  callback_type member_callback2 = callback_type::create<Test, test, &Test::callback2>();

  //***************************************************************************
  // Free function callback via etl::function
  //***************************************************************************
  std::vector<uint64_t> free_tick_list1;

  void free_callback1()
  {
    free_tick_list1.push_back(ticks);
  }

  callback_type free_function_callback = callback_type::create<free_callback1>();

  //***************************************************************************
  // Free function callback via function pointer
  //***************************************************************************
  std::vector<uint64_t> free_tick_list2;

  void free_callback2()
  {
    free_tick_list2.push_back(ticks);
  }

  callback_type free_function_callback2 = callback_type::create<free_callback2>();

  SUITE(test_callback_timer_interrupt)
  {
    //*************************************************************************
    TEST(callback_timer_interrupt_too_many_timers)
    {
      etl::callback_timer_interrupt<2, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Single_Shot);

      CHECK(id1 != etl::timer::id::NO_TIMER);
      CHECK(id2 != etl::timer::id::NO_TIMER);
      CHECK(id3 == etl::timer::id::NO_TIMER);

      timer_controller.clear();
      id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Single_Shot);
      CHECK(id3 != etl::timer::id::NO_TIMER);

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_one_shot)
    {
      etl::callback_timer_interrupt<4, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Single_Shot);

      test.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1UL;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 37 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11 };

      CHECK(test.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_one_shot_after_timeout)
    {
      etl::callback_timer_interrupt<1, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Single_Shot);
      test.tick_list.clear();

      timer_controller.start(id1);
      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      // Timer should have timed out.

      CHECK(timer_controller.set_period(id1, 50));
      timer_controller.start(id1);

      test.tick_list.clear();

      ticks = 0;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      // Timer should have timed out.

      CHECK_EQUAL(50U, *test.tick_list.data());

      CHECK(timer_controller.unregister_timer(id1));
      CHECK(!timer_controller.unregister_timer(id1));
      CHECK(!timer_controller.start(id1));
      CHECK(!timer_controller.stop(id1));

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_repeating)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      test.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 37, 74 };
      std::vector<uint64_t> compare2 = { 23, 46, 69, 92 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK(test.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_repeating_bigger_step)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      test.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      CHECK(!timer_controller.is_running());

      timer_controller.enable(true);

      CHECK(timer_controller.is_running());

      ticks = 0;

      const uint32_t step = 5U;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 40, 75 };
      std::vector<uint64_t> compare2 = { 25, 50, 70, 95 };
      std::vector<uint64_t> compare3 = { 15, 25, 35, 45, 55, 70, 80, 90, 100 };

      CHECK(test.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_repeating_stop_start)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      test.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        if (ticks == 40)
        {
          timer_controller.start(id1);
          timer_controller.stop(id2);
        }

        if (ticks == 80)
        {
          timer_controller.stop(id1);
          timer_controller.start(id2);
        }

        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 77 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK(test.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_timer_starts_timer_small_step)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback2, 100, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(member_callback, 10, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(member_callback, 22, etl::timer::mode::Single_Shot);

      (void)id2;
      (void)id3;

      test.set_controller(timer_controller);

      test.tick_list.clear();

      timer_controller.start(id1);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1U;

      while (ticks <= 200U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 100, 110, 122 };

      CHECK(test.tick_list.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(), compare1.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_timer_starts_timer_big_step)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback2, 100, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(member_callback,   10, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(member_callback,   22, etl::timer::mode::Single_Shot);

      (void)id2;
      (void)id3;

      test.set_controller(timer_controller);

      test.tick_list.clear();

      timer_controller.start(id1);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 3;

      while (ticks <= 200U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 102, 111, 123 };

      CHECK(test.tick_list.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_repeating_register_unregister)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1;
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      test.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        if (ticks == 40)
        {
          timer_controller.unregister_timer(id2);

          id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Repeating);
          timer_controller.start(id1);
        }

        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 77 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK(test.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_repeating_clear)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      test.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        ticks += step;

        if (ticks == 40)
        {
          timer_controller.clear();
        }

        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 37 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11, 22, 33 };

      CHECK(test.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_delayed_immediate)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      test.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.enable(true);

      ticks = 5;
      timer_controller.tick(uint32_t(ticks));

      timer_controller.start(id1, etl::timer::start::Immediate);
      timer_controller.start(id2, etl::timer::start::Immediate);
      timer_controller.start(id3, etl::timer::start::Delayed);

      const uint32_t step = 1U;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 6, 42, 79 };
      std::vector<uint64_t> compare2 = { 6, 28, 51, 74, 97 };
      std::vector<uint64_t> compare3 = { 16, 27, 38, 49, 60, 71, 82, 93 };

      CHECK(test.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), test.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_one_shot_big_step_short_delay_insert)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(free_function_callback,  15, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback2, 5,  etl::timer::mode::Repeating);

      free_tick_list1.clear();
      free_tick_list2.clear();

      timer_controller.start(id1);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 11U;

      ticks += step;
      timer_controller.tick(step);

      ticks += step;
      timer_controller.tick(step);

      std::vector<uint64_t> compare1 = { 22 };
      std::vector<uint64_t> compare2 = { 11, 11, 22, 22 };

      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), free_tick_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list2.data(), compare2.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_one_shot_empty_list_huge_tick_before_insert)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(free_function_callback, 5, etl::timer::mode::Single_Shot);

      free_tick_list1.clear();

      timer_controller.start(id1);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 5U;

      for (uint32_t i = 0U; i < step; ++i)
      {
        ++ticks;
        timer_controller.tick(1);
      }

      // Huge tick count.
      timer_controller.tick(UINT32_MAX - step + 1);

      timer_controller.start(id1);

      for (uint32_t i = 0U; i < step; ++i)
      {
        ++ticks;
        timer_controller.tick(1);
      }
      std::vector<uint64_t> compare1 = { 5, 10 };

      CHECK(free_tick_list1.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), free_tick_list1.data(), compare1.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_time_to_next)
    {
      etl::callback_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      CHECK_EQUAL(11, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(4, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(8, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(1, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(5, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(2, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(2, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(6, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(10, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(3, timer_controller.time_to_next());

      timer_controller.tick(7);
      CHECK_EQUAL(4, timer_controller.time_to_next());
    }

    //*************************************************************************
    class test_object
    {
    public:

      void call()
      {
        ++called;
      }

      size_t called = 0UL;
    };

    TEST(callback_timer_interrupt_call_etl_delegate)
    {
      test_object test_obj;
      callback_type delegate_callback = callback_type::create<test_object, &test_object::call>(test_obj);
        
      etl::callback_timer_interrupt<1, ScopedGuard> timer_controller;

      timer_controller.enable(true);

      etl::timer::id::type id = timer_controller.register_timer(delegate_callback, 5, etl::timer::mode::Single_Shot);
      timer_controller.start(id);

      timer_controller.tick(4);
      CHECK(test_obj.called == 0);

      timer_controller.tick(2);
      CHECK(test_obj.called == 1);

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(callback_timer_interrupt_log_timer_calls)
    {
      std::vector<TimerLogEntry> timer_log;

      etl::callback_timer_interrupt<4, ScopedGuard> timer_controller;
      size_t timer_count = 0U;

      // Create the callbacks.
      static auto dc0 = [&]()
      {
        timer_log.push_back(TimerLogEntry{ 0, timer_count });
      };

      static auto dc1 = [&]()
      {
        timer_log.push_back(TimerLogEntry{ 1, timer_count });
      };

      static auto dc2 = [&]()
      {
        timer_log.push_back(TimerLogEntry{ 2, timer_count });
      };

      static auto dc3 = [&]()
      {
        timer_log.push_back(TimerLogEntry{ 3, timer_count });
      };

      callback_type delegate_callback0(dc0);
      
      callback_type delegate_callback1(dc1);
      
      callback_type delegate_callback2(dc2);
      
      callback_type delegate_callback3(dc3);

      timer_log.clear();
      timer_controller.enable(true);

      constexpr uint32_t T0 = 2U;
      constexpr uint32_t T1 = 3U;
      constexpr uint32_t T2 = 4U;
      constexpr uint32_t T3 = 5U;

      // Register the timers.
      etl::timer::id::type id0 = timer_controller.register_timer(delegate_callback0, T0, etl::timer::mode::Repeating);
      etl::timer::id::type id1 = timer_controller.register_timer(delegate_callback1, T1, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(delegate_callback2, T2, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(delegate_callback3, T3, etl::timer::mode::Repeating);
      
      // Start the repeating timers.
      timer_controller.start(id0);
      timer_controller.start(id1);
      timer_controller.start(id2);
      timer_controller.start(id3);

      // Run the timer.
      for (int i = 1; i < 50; ++i)
      {
        ++timer_count;
        timer_controller.tick(1);
      }

      // Check the results log.
      for (auto& t : timer_log)
      {
        switch (t.id)
        {
          case 0:
          {
            CHECK_EQUAL(0, t.time_called % 2);
            break;
          }

          case 1:
          {
            CHECK_EQUAL(0, (t.time_called % 5) % 3);
            break;
          }

          case 2:
          {
            CHECK_EQUAL(0, t.time_called % 4);
            break;
          }

          case 3:
          {
            CHECK_EQUAL(0, t.time_called % 5);
            break;
          }

          default:
          {
            CHECK(false);
            break;
          }
        }
      }

      // Check the 
      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }
  };
}
