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

#include "etl/callback_timer_locked.h"
#include "etl/delegate.h"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

#if defined(ETL_COMPILER_MICROSOFT)
  #include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  uint64_t ticks = 0ULL;

  //***************************************************************************
  struct Locks
  {
    Locks()
      : lock_count(0)
    {
    }

    void clear()
    {
      lock_count = 0;
    }

    bool try_lock()
    {
      ++lock_count;
      return true;
    }

    void lock()
    {
      ++lock_count;
    }

    void unlock()
    {
      --lock_count;
    }

    int lock_count;
  };

  Locks locks;

  //***************************************************************************
  // Class callback via etl::function
  //***************************************************************************
  class Object
  {
  public:

    Object()
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

    void set_controller(etl::callback_timer_locked<3>& controller)
    {
      p_controller = &controller;
    }

    std::vector<uint64_t> tick_list;

    etl::callback_timer_locked<3>* p_controller;
  };

  using callback_type = etl::icallback_timer_locked::callback_type;
  using try_lock_type = etl::icallback_timer_locked::try_lock_type;
  using lock_type     = etl::icallback_timer_locked::lock_type;
  using unlock_type   = etl::icallback_timer_locked::unlock_type;

  Object object;
  callback_type member_callback  = callback_type::create<Object, object, &Object::callback>();
  callback_type member_callback2 = callback_type::create<Object, object, &Object::callback2>();

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

  SUITE(test_callback_timer_locked)
  {
    //*************************************************************************
    TEST(callback_timer_locked_too_many_timers)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<2> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Single_Shot);

      CHECK(id1 != etl::timer::id::NO_TIMER);
      CHECK(id2 != etl::timer::id::NO_TIMER);
      CHECK(id3 == etl::timer::id::NO_TIMER);

      timer_controller.clear();
      id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Single_Shot);
      CHECK(id3 != etl::timer::id::NO_TIMER);

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_one_shot)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<4> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Single_Shot);

      object.tick_list.clear();
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

      CHECK(object.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(message_timer_one_shot_after_timeout)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<1> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Single_Shot);
      object.tick_list.clear();

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

      object.tick_list.clear();

      ticks = 0;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      // Timer should have timed out.

      CHECK_EQUAL(50U, *object.tick_list.data());

      CHECK(timer_controller.unregister_timer(id1));
      CHECK(!timer_controller.unregister_timer(id1));
      CHECK(!timer_controller.start(id1));
      CHECK(!timer_controller.stop(id1));

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_repeating)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      object.tick_list.clear();
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

      CHECK(object.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_repeating_bigger_step)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      object.tick_list.clear();
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

      CHECK(object.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_repeating_stop_start)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type lock         = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type unlock     = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      object.tick_list.clear();
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

      CHECK(object.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_timer_starts_timer_small_step)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback2, 100, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(member_callback, 10, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(member_callback, 22, etl::timer::mode::Single_Shot);

      (void)id2;
      (void)id3;

      object.set_controller(timer_controller);

      object.tick_list.clear();

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

      CHECK(object.tick_list.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(), compare1.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_timer_starts_timer_big_step)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback2, 100, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(member_callback,   10, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(member_callback,   22, etl::timer::mode::Single_Shot);

      (void)id2;
      (void)id3;

      object.set_controller(timer_controller);

      object.tick_list.clear();

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

      CHECK(object.tick_list.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_repeating_register_unregister)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1;
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      object.tick_list.clear();
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

      CHECK(object.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_repeating_clear)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      object.tick_list.clear();
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

      CHECK(object.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_delayed_immediate)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback,         37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback,  23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Repeating);

      object.tick_list.clear();
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

      CHECK(object.tick_list.size() != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), free_tick_list2.data(), compare3.size());

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_one_shot_big_step_short_delay_insert)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

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

      CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(callback_timer_locked_one_shot_empty_list_huge_tick_before_insert)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

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

      CHECK_EQUAL(0U, locks.lock_count);
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

    TEST(callback_timer_locked_call_etl_delegate)
    {
        test_object test_obj;
        callback_type delegate_callback = callback_type::create<test_object, &test_object::call>(test_obj);
        
        locks.clear();
        try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
        lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
        unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

        etl::callback_timer_locked<1> timer_controller(try_lock, lock, unlock);

        timer_controller.enable(true);

        etl::timer::id::type id = timer_controller.register_timer(delegate_callback, 5, etl::timer::mode::Single_Shot);
        timer_controller.start(id);

        timer_controller.tick(4);
        CHECK(test_obj.called == 0);

        timer_controller.tick(2);
        CHECK(test_obj.called == 1);

        CHECK_EQUAL(0U, locks.lock_count);
    }

    //*************************************************************************
    TEST(message_timer_time_to_next)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

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
    TEST(message_timer_time_to_next_with_has_active_timer)
    {
      locks.clear();
      try_lock_type try_lock = try_lock_type::create<Locks, locks, &Locks::try_lock>();
      lock_type     lock     = lock_type::create<Locks, locks, &Locks::lock>();
      unlock_type   unlock   = unlock_type::create<Locks, locks, &Locks::unlock>();

      etl::callback_timer_locked<3> timer_controller(try_lock, lock, unlock);

      etl::timer::id::type id1 = timer_controller.register_timer(member_callback, 37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(free_function_callback, 23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(free_function_callback2, 11, etl::timer::mode::Single_Shot);

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      timer_controller.tick(11);
      CHECK_EQUAL(12, timer_controller.time_to_next());
      CHECK_TRUE(timer_controller.has_active_timer());

      timer_controller.tick(23);
      CHECK_EQUAL(3, timer_controller.time_to_next());
      CHECK_TRUE(timer_controller.has_active_timer());

      timer_controller.tick(2);
      CHECK_EQUAL(1, timer_controller.time_to_next());
      CHECK_TRUE(timer_controller.has_active_timer());

      timer_controller.tick(1);
      CHECK_EQUAL(static_cast<etl::timer::interval::type>(etl::timer::interval::No_Active_Interval), timer_controller.time_to_next());
      CHECK_FALSE(timer_controller.has_active_timer());
    }

    //*************************************************************************
#if REALTIME_TEST

  #if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
    #define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
    #define FIX_PROCESSOR_AFFINITY SetThreadAffinityMask(GetCurrentThread(), 1);
  #else
    #define RAISE_THREAD_PRIORITY
    #define FIX_PROCESSOR_AFFINITY
  #endif

    etl::callback_timer_locked<3> controller;

    //*********************************
    struct ThreadLock
    {
      ThreadLock()
        : lock_count(0)
      {
      }

      bool try_lock()
      {
        if (mutex.try_lock())
        {
          ++lock_count;

          return true;
        }

        return false;
      }

      void lock()
      {
        mutex.lock();
        ++lock_count;
      }

      void unlock()
      {
        mutex.unlock();
        --lock_count;
      }

      std::mutex mutex;
      int lock_count;
    };

    ThreadLock threadLock;

    //*********************************
    void timer_event()
    {
      const uint32_t TICK = 1U;
      uint32_t tick = TICK;
      ticks = 1U;

      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY;

      while (ticks <= 1000U)
      {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (controller.tick(tick))
        {
          tick = TICK;
        }
        else
        {
          tick += TICK;
        }

        ++ticks;
      }
    }

    TEST(callback_timer_locked_threads)
    {
      FIX_PROCESSOR_AFFINITY;

      etl::timer::id::type id1 = controller.register_timer(member_callback,         400, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = controller.register_timer(free_function_callback,  100, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = controller.register_timer(free_function_callback2,  10, etl::timer::mode::Repeating);

      object.tick_list.clear();
      free_tick_list1.clear();
      free_tick_list2.clear();

      try_lock_type try_lock_callback = try_lock_type::create<ThreadLock, threadLock, &ThreadLock::try_lock>();
      lock_type     lock_callback     = lock_type::create<ThreadLock, threadLock, &ThreadLock::lock>();
      unlock_type   unlock_callback   = unlock_type::create<ThreadLock, threadLock, &ThreadLock::unlock>();

      controller.set_locks(try_lock_callback, lock_callback, unlock_callback);

      controller.start(id1);
      controller.start(id2);
      //controller.start(id3);

      controller.enable(true);

      std::thread t1(timer_event);

      bool restart_1 = true;

      while (ticks <= 1000U)
      {
        if ((ticks > 200U) && (ticks < 500U))
        {
          controller.stop(id3);
        }

        if ((ticks > 600U) && (ticks < 800U))
        {
          controller.start(id3);
        }

        if ((ticks > 500U) && restart_1)
        {
          controller.start(id1);
          restart_1 = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      //Join the thread with the main thread
      t1.join();

      CHECK_EQUAL(2U,  object.tick_list.size());
      CHECK_EQUAL(10U, free_tick_list1.size());
      CHECK(free_tick_list2.size() < 65U);

      std::vector<uint64_t> compare1 = { 400, 900 };
      std::vector<uint64_t> compare2 = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

      CHECK(object.tick_list.size()  != 0);
      CHECK(free_tick_list1.size() != 0);
      CHECK(free_tick_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), object.tick_list.data(),  min(compare1.size(), object.tick_list.size()));
      CHECK_ARRAY_EQUAL(compare2.data(), free_tick_list1.data(), min(compare2.size(), free_tick_list1.size()));

      CHECK_EQUAL(0U, threadLock.lock_count);
    }
#endif
  };
}
