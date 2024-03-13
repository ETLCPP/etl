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

#include "etl/message_router.h"
#include "etl/message_bus.h"
#include "etl/message_timer_interrupt.h"

#include <iostream>
#include <vector>

//***************************************************************************
// The set of messages.
//***************************************************************************
namespace
{
  uint64_t ticks = 0;

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
  enum
  {
    MESSAGE1,
    MESSAGE2,
    MESSAGE3,
    MESSAGE4
  };

  enum
  {
    ROUTER1 = 1,
  };

  struct Message1 : public etl::message<MESSAGE1>
  {
  };

  struct Message2 : public etl::message<MESSAGE2>
  {
  };

  struct Message3 : public etl::message<MESSAGE3>
  {
  };

  struct Message4 : public etl::message<MESSAGE4>
  {
  };

  Message1 message1;
  Message2 message2;
  Message3 message3;
  Message4 message4;

  //***************************************************************************
  // Router that handles messages 1, 2, 3
  //***************************************************************************
  class Router1 : public etl::message_router<Router1, Message1, Message2, Message3>
  {
  public:

    Router1()
      : message_router(ROUTER1)
    {
    }

    void on_receive(const Message1&)
    {
      message1.push_back(ticks);
    }

    void on_receive(const Message2&)
    {
      message2.push_back(ticks);
    }

    void on_receive(const Message3&)
    {
      message3.push_back(ticks);
    }

    void on_receive_unknown(const etl::imessage&)
    {
    }

    void clear()
    {
      message1.clear();
      message2.clear();
      message3.clear();
    }

    std::vector<uint64_t> message1;
    std::vector<uint64_t> message2;
    std::vector<uint64_t> message3;
  };

  //***************************************************************************
  // Bus that handles messages 1, 2, 3
  //***************************************************************************
  class Bus1 : public etl::message_bus<1>
  {

  };

  //***********************************
  Router1 router1;
  Bus1    bus1;

  SUITE(test_message_timer_interrupt)
  {
    //*************************************************************************
    TEST(message_timer_too_many_timers)
    {
      etl::message_timer_interrupt<2, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Single_Shot);

      CHECK(id1 != etl::timer::id::NO_TIMER);
      CHECK(id2 != etl::timer::id::NO_TIMER);
      CHECK(id3 == etl::timer::id::NO_TIMER);

      timer_controller.clear();
      id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Single_Shot);
      CHECK(id3 != etl::timer::id::NO_TIMER);

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_one_shot)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Single_Shot);

      router1.clear();

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

      std::vector<uint64_t> compare1 = { 37ULL };
      std::vector<uint64_t> compare2 = { 23ULL };
      std::vector<uint64_t> compare3 = { 11ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_one_shot_after_timeout)
    {
      etl::message_timer_interrupt<1, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Single_Shot);
      router1.clear();

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

      router1.clear();

      ticks = 0;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      // Timer should have timed out.

      CHECK_EQUAL(50U, *router1.message1.data());

      CHECK(timer_controller.unregister_timer(id1));
      CHECK(!timer_controller.unregister_timer(id1));
      CHECK(!timer_controller.start(id1));
      CHECK(!timer_controller.stop(id1));

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_repeating)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Repeating);

      router1.clear();

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

      std::vector<uint64_t> compare1 = { 37ULL, 74ULL };
      std::vector<uint64_t> compare2 = { 23ULL, 46ULL, 69ULL, 92ULL };
      std::vector<uint64_t> compare3 = { 11ULL, 22ULL, 33ULL, 44ULL, 55ULL, 66ULL, 77ULL, 88ULL, 99ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_repeating_bigger_step)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Repeating);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      CHECK(!timer_controller.is_running());

      timer_controller.enable(true);

      CHECK(timer_controller.is_running());

      ticks = 0;

      const uint32_t step = 5UL;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 40ULL, 75ULL };
      std::vector<uint64_t> compare2 = { 25ULL, 50ULL, 70ULL, 95ULL };
      std::vector<uint64_t> compare3 = { 15ULL, 25ULL, 35ULL, 45ULL, 55ULL, 70ULL, 80ULL, 90ULL, 100ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_repeating_stop_start)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Repeating);

      router1.clear();

      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1UL;

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

      std::vector<uint64_t> compare1 = { 77ULL };
      std::vector<uint64_t> compare2 = { 23ULL };
      std::vector<uint64_t> compare3 = { 11ULL, 22ULL, 33ULL, 44ULL, 55ULL, 66ULL, 77ULL, 88ULL, 99ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_repeating_register_unregister)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1;
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Repeating);

      router1.clear();

      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1UL;

      while (ticks <= 100U)
      {
        if (ticks == 40)
        {
          timer_controller.unregister_timer(id2);

          id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Repeating);
          timer_controller.start(id1);
        }

        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 77ULL };
      std::vector<uint64_t> compare2 = { 23ULL };
      std::vector<uint64_t> compare3 = { 11ULL, 22ULL, 33ULL, 44ULL, 55ULL, 66ULL, 77ULL, 88ULL, 99ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_repeating_clear)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Repeating);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

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

      std::vector<uint64_t> compare1 = { 37ULL };
      std::vector<uint64_t> compare2 = { 23ULL };
      std::vector<uint64_t> compare3 = { 11ULL, 22ULL, 33ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_route_through_bus)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, bus1, 37, etl::timer::mode::Single_Shot, ROUTER1);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, bus1, 23, etl::timer::mode::Single_Shot, ROUTER1);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, bus1, 11, etl::timer::mode::Single_Shot, etl::imessage_router::ALL_MESSAGE_ROUTERS);

      bus1.subscribe(router1);

      router1.clear();

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

      std::vector<uint64_t> compare1 = { 37ULL };
      std::vector<uint64_t> compare2 = { 23ULL };
      std::vector<uint64_t> compare3 = { 11ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_immediate_delayed)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Repeating);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 5;
      timer_controller.tick(uint32_t(ticks));

      timer_controller.start(id1, etl::timer::start::Immediate);
      timer_controller.start(id2, etl::timer::start::Immediate);
      timer_controller.start(id3, etl::timer::start::Delayed);

      const uint32_t step = 1UL;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 6ULL, 42ULL, 79ULL };
      std::vector<uint64_t> compare2 = { 6ULL, 28ULL, 51ULL, 74ULL, 97ULL };
      std::vector<uint64_t> compare3 = { 16ULL, 27ULL, 38ULL, 49ULL, 60ULL, 71ULL, 82ULL, 93ULL };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_one_shot_big_step_short_delay_insert)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 15, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1,  5, etl::timer::mode::Repeating);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 11UL;

      ticks += step;
      timer_controller.tick(step);

      ticks += step;
      timer_controller.tick(step);

      std::vector<uint64_t> compare1 = { 22 };
      std::vector<uint64_t> compare2 = { 11, 11, 22, 22 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_one_shot_empty_list_huge_tick_before_insert)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 5, etl::timer::mode::Single_Shot);

      router1.clear();

      timer_controller.start(id1);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 5ULL;

      for (uint32_t i = 0UL; i < step; ++i)
      {
        ++ticks;
        timer_controller.tick(1);
      }

      // Huge tick count.
      timer_controller.tick(UINT32_MAX - step + 1);

      timer_controller.start(id1);

      for (uint32_t i = 0UL; i < step; ++i)
      {
        ++ticks;
        timer_controller.tick(1);
      }
      std::vector<uint64_t> compare1 = { 5, 10 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());

      CHECK_EQUAL(0U, ScopedGuard::guard_count);
    }

    //*************************************************************************
    TEST(message_timer_time_to_next)
    {
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Repeating);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Repeating);

      router1.clear();

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
      etl::message_timer_interrupt<3, ScopedGuard> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::Single_Shot);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::Single_Shot);

      router1.clear();

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
    class RouterLog : public etl::message_router<RouterLog, Message1, Message2, Message3, Message4>
    {
    public:

      //*********************************
      RouterLog(etl::imessage_timer_interrupt<ScopedGuard>& timer_controller_)
        : message_router(ROUTER1)
        , timer_count(0)
        , timer_controller(timer_controller_)
      {
      }

      //*********************************
      void on_receive(const Message1&)
      {
        // Id0
        timer_log.push_back(TimerLogEntry{ 0, timer_count });
      }

      //*********************************
      void on_receive(const Message2&)
      {
        // Id1
        timer_log.push_back(TimerLogEntry{ 1, timer_count });
      }

      //*********************************
      void on_receive(const Message3&)
      {
        // Id2
        timer_log.push_back(TimerLogEntry{ 2, timer_count });
      }

      //*********************************
      void on_receive(const Message4&)
      {
        // Id3
        timer_log.push_back(TimerLogEntry{ 3, timer_count });
        timer_controller.start(1);
      }

      //*********************************
      void on_receive_unknown(const etl::imessage&)
      {
        timer_log.push_back(TimerLogEntry{ 99, timer_count });
      }

      size_t timer_count;
      std::vector<TimerLogEntry> timer_log;
      etl::imessage_timer_interrupt<ScopedGuard>& timer_controller;
    };

    TEST(message_timer_interrupt_log_timer_calls)
    {   
      etl::message_timer_interrupt<4, ScopedGuard> timer_controller;
      RouterLog router(timer_controller);

      timer_controller.enable(true);

      constexpr uint32_t T1 = 2U;
      constexpr uint32_t T2 = 3U;
      constexpr uint32_t T3 = 4U;
      constexpr uint32_t T4 = 5U;

      // Register the timers.
      etl::timer::id::type id1 = timer_controller.register_timer(message1, router, T1, etl::timer::mode::Repeating);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router, T2, etl::timer::mode::Single_Shot);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router, T3, etl::timer::mode::Repeating);
      etl::timer::id::type id4 = timer_controller.register_timer(message4, router, T4, etl::timer::mode::Repeating);

      // Start the repeating timers.
      timer_controller.start(id1);
      timer_controller.start(id2);
      timer_controller.start(id3);
      timer_controller.start(id4);

      // Run the timer.
      for (int i = 1; i < 50; ++i)
      {
        ++router.timer_count;
        timer_controller.tick(1);
      }

      // Check the results log.
      for (auto t : router.timer_log)
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
