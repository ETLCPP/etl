/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#include "UnitTest++.h"
#include "ExtraCheckMacros.h"

#include "message_router.h"
#include "message_bus.h"
#include "message_timer.h"

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#if defined(ETL_COMPILER_MICROSOFT)
#include <Windows.h>
#endif

#define REALTIME_TEST 0

//***************************************************************************
// The set of messages.
//***************************************************************************
namespace
{
  uint64_t ticks = 0;

  enum
  {
    MESSAGE1,
    MESSAGE2,
    MESSAGE3,
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

  Message1 message1;
  Message2 message2;
  Message3 message3;

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

    void on_receive(etl::imessage_router&, const Message1&)
    {
      message1.push_back(ticks);
    }

    void on_receive(etl::imessage_router&, const Message2&)
    {
      message2.push_back(ticks);
    }

    void on_receive(etl::imessage_router&, const Message3&)
    {
      message3.push_back(ticks);
    }

    void on_receive_unknown(etl::imessage_router&, const etl::imessage&)
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

  SUITE(test_message_timer)
  {
    //=========================================================================
    TEST(message_timer_too_many_timers)
    {
      etl::message_timer<2> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::SINGLE_SHOT);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::SINGLE_SHOT);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::SINGLE_SHOT);

      CHECK(id1 != etl::timer::id::NO_TIMER);
      CHECK(id2 != etl::timer::id::NO_TIMER);
      CHECK(id3 == etl::timer::id::NO_TIMER);

      timer_controller.clear();
      id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::SINGLE_SHOT);
      CHECK(id3 != etl::timer::id::NO_TIMER);
    }

    //=========================================================================
    TEST(message_timer_one_shot)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::SINGLE_SHOT);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::SINGLE_SHOT);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::SINGLE_SHOT);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 37 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_repeating)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::REPEATING);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::REPEATING);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::REPEATING);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 37, 74 };
      std::vector<uint64_t> compare2 = { 23, 46, 69, 92 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_repeating_bigger_step)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::REPEATING);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::REPEATING);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::REPEATING);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      CHECK(!timer_controller.is_running());

      timer_controller.enable(true);

      CHECK(timer_controller.is_running());

      ticks = 0;

      const uint32_t step = 5;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 40, 75 };
      std::vector<uint64_t> compare2 = { 25, 50, 70, 95 };
      std::vector<uint64_t> compare3 = { 15, 25, 35, 45, 55, 70, 80, 90, 100 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_repeating_stop_start)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::REPEATING);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::REPEATING);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::REPEATING);

      router1.clear();

      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1;

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

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_repeating_register_unregister)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1;
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::REPEATING);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::REPEATING);

      router1.clear();

      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        if (ticks == 40)
        {
          timer_controller.unregister_timer(id2);

          id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::REPEATING);
          timer_controller.start(id1);
        }

        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 77 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_repeating_clear)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::REPEATING);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::REPEATING);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::REPEATING);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1;

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

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_route_through_bus)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, bus1, 37, etl::timer::mode::SINGLE_SHOT, ROUTER1);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, bus1, 23, etl::timer::mode::SINGLE_SHOT, ROUTER1);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, bus1, 11, etl::timer::mode::SINGLE_SHOT, etl::imessage_router::ALL_MESSAGE_ROUTERS);

      bus1.subscribe(router1);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 37 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_immediate_delayed)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 37, etl::timer::mode::REPEATING);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1, 23, etl::timer::mode::REPEATING);
      etl::timer::id::type id3 = timer_controller.register_timer(message3, router1, 11, etl::timer::mode::REPEATING);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id3);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 5;
      timer_controller.tick(uint32_t(ticks));

      timer_controller.start(id1, etl::timer::start::IMMEDIATE);
      timer_controller.start(id2, etl::timer::start::IMMEDIATE);
      timer_controller.start(id3, etl::timer::start::DELAYED);

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;
        timer_controller.tick(step);
      }

      std::vector<uint64_t> compare1 = { 6, 42, 79 };
      std::vector<uint64_t> compare2 = { 6, 28, 51, 74, 97 };
      std::vector<uint64_t> compare3 = { 16, 27, 38, 49, 60, 71, 82, 93 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), router1.message3.data(), compare3.size());
    }

    //=========================================================================
    TEST(message_timer_one_shot_big_step_short_delay_insert)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 15, etl::timer::mode::SINGLE_SHOT);
      etl::timer::id::type id2 = timer_controller.register_timer(message2, router1,  5, etl::timer::mode::REPEATING);

      router1.clear();

      timer_controller.start(id1);
      timer_controller.start(id2);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 11;

      ticks += step;
      timer_controller.tick(step);

      ticks += step;
      timer_controller.tick(step);

      std::vector<uint64_t> compare1 = { 22 };
      std::vector<uint64_t> compare2 = { 11, 11, 22, 22 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), router1.message2.data(), compare2.size());
    }

    //=========================================================================
    TEST(callback_timer_one_shot_empty_list_huge_tick_before_insert)
    {
      etl::message_timer<3> timer_controller;

      etl::timer::id::type id1 = timer_controller.register_timer(message1, router1, 5, etl::timer::mode::SINGLE_SHOT);

      router1.clear();

      timer_controller.start(id1);

      timer_controller.enable(true);

      ticks = 0;

      const uint32_t step = 5;

      for (uint32_t i = 0; i < step; ++i)
      {
        ++ticks;
        timer_controller.tick(1);
      }

      // Huge tick count.
      timer_controller.tick(UINT32_MAX - step + 1);

      timer_controller.start(id1);

      for (uint32_t i = 0; i < step; ++i)
      {
        ++ticks;
        timer_controller.tick(1);
      }
      std::vector<uint64_t> compare1 = { 5, 10 };

      CHECK_ARRAY_EQUAL(compare1.data(), router1.message1.data(), compare1.size());
    }

    //=========================================================================
#if REALTIME_TEST

  #if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
    #define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
    #define FIX_PROCESSOR_AFFINITY SetThreadAffinityMask(GetCurrentThread(), 1);
  #else
    #error No thread priority modifier defined
  #endif

    etl::message_timer<3> controller;

    void timer_event()
    {
      const uint32_t TICK = 1;
      uint32_t tick = TICK;
      ticks = 1;

      RAISE_THREAD_PRIORITY;
      FIX_PROCESSOR_AFFINITY;

      while (ticks <= 1000)
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

    TEST(message_timer_threads)
    {
      FIX_PROCESSOR_AFFINITY;

      etl::timer::id::type id1 = controller.register_timer(message1, router1, 400,  etl::timer::mode::SINGLE_SHOT);
      etl::timer::id::type id2 = controller.register_timer(message2, router1, 100,  etl::timer::mode::REPEATING);
      etl::timer::id::type id3 = controller.register_timer(message3, router1, 10,   etl::timer::mode::REPEATING);

      router1.clear();

      controller.start(id1);
      controller.start(id2);
      controller.start(id3);

      controller.enable(true);

      std::thread t1(timer_event);

      bool restart_1 = true;

      while (ticks < 1000U)
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

      CHECK_EQUAL(2U,  router1.message1.size());
      CHECK_EQUAL(10U, router1.message2.size());
      CHECK(router1.message2.size() < 65U);
    }
#endif
  };
}
