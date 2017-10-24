/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

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

#include "platform.h"

extern "C"
{
  #include "ecl_timer.h"
}

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#if defined(ETL_COMPILER_MICROSOFT)
#include <Windows.h>
#endif

#define REALTIME_TEST 0

namespace
{
  uint64_t ticks = 0;

  std::vector<uint64_t> callback_list1;
  std::vector<uint64_t> callback_list2;
  std::vector<uint64_t> callback_list3;

  void callback1()
  {
    callback_list1.push_back(ticks);
  }

  void callback2()
  {
    callback_list2.push_back(ticks);
  }

  void callback3()
  {
    callback_list3.push_back(ticks);
  }

  void callback3b()
  {
    callback_list3.push_back(ticks);

    ecl_timer_start(2, ECL_TIMER_START_DELAYED);
    ecl_timer_start(1, ECL_TIMER_START_DELAYED);
  }

  const int NTIMERS = 3;
  struct ecl_timer_config timers[NTIMERS];

  SUITE(test_ecl_timer)
  {
    //=========================================================================
    TEST(ecl_timer_too_many_timers)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 37, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id4 = ecl_timer_register(callback3, 11, ECL_TIMER_SINGLE_SHOT);

      CHECK(id1 != ECL_TIMER_NO_TIMER);
      CHECK(id2 != ECL_TIMER_NO_TIMER);
      CHECK(id3 != ECL_TIMER_NO_TIMER);
      CHECK(id4 == ECL_TIMER_NO_TIMER);

      ecl_timer_clear();
      id3 = ecl_timer_register(callback3, 11, ECL_TIMER_SINGLE_SHOT);
      CHECK(id3 != ECL_TIMER_NO_TIMER);
    }

    //=========================================================================
    TEST(ecl_timer_one_shot)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 37, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_SINGLE_SHOT);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id3, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;
      
      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare1 = { 37 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_repeating)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 37, ECL_TIMER_REPEATING);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_REPEATING);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id3, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare1 = { 37, 74 };
      std::vector<uint64_t> compare2 = { 23, 46, 69, 92 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_repeating_bigger_step)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 37, ECL_TIMER_REPEATING);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_REPEATING);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id3, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 5;

      while (ticks <= 100U)
      {
        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare1 = { 40, 75 };
      std::vector<uint64_t> compare2 = { 25, 50, 70, 95 };
      std::vector<uint64_t> compare3 = { 15, 25, 35, 45, 55, 70, 80, 90, 100 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_repeating_stop_start)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 37, ECL_TIMER_REPEATING);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_REPEATING);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id3, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        if (ticks == 40)
        {
          ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
          ecl_timer_stop(id2);
        }

        if (ticks == 80)
        {
          ecl_timer_stop(id1);
          ecl_timer_start(id2, ECL_TIMER_START_DELAYED);
        }

        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare1 = { 77 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };
            
      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_timer_starts_timer_small_step)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback3b, 100, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id2 = ecl_timer_register(callback3,   10, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id3 = ecl_timer_register(callback3,   22, ECL_TIMER_SINGLE_SHOT);

      (void)id2;
      (void)id3;

      callback_list3.clear();

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 200U)
      {
        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare3 = { 100, 110, 122 };

      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_timer_starts_timer_big_step)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback3b, 100, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id2 = ecl_timer_register(callback3,   10, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id3 = ecl_timer_register(callback3,   22, ECL_TIMER_SINGLE_SHOT);

      (void)id2;
      (void)id3;

      callback_list3.clear();

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 3;

      while (ticks <= 200U)
      {
        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare3 = { 102, 111, 123 };

      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }


    //=========================================================================
    TEST(ecl_timer_repeating_register_unregister)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1;
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_REPEATING);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_start(id3, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        if (ticks == 40)
        {         
          ecl_timer_unregister(id2);

          id1 = ecl_timer_register(callback1, 37, ECL_TIMER_REPEATING);
          ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
        }

        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare1 = { 77 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_repeating_clear)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 37, ECL_TIMER_REPEATING);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_REPEATING);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id3, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;

        if (ticks == 40)
        {
          ecl_timer_clear();
        }

        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare1 = { 37 };
      std::vector<uint64_t> compare2 = { 23 };
      std::vector<uint64_t> compare3 = { 11, 22, 33, 44, 55, 66, 77, 88, 99 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_delayed_immediate)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 37, ECL_TIMER_REPEATING);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 23, ECL_TIMER_REPEATING);
      ecl_timer_id_t id3 = ecl_timer_register(callback3, 11, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 5;
      ecl_timer_tick(uint32_t(ticks));

      ecl_timer_start(id1, ECL_TIMER_START_IMMEDIATE);
      ecl_timer_start(id2, ECL_TIMER_START_IMMEDIATE);
      ecl_timer_start(id3, ECL_TIMER_START_DELAYED);

      const uint32_t step = 1;

      while (ticks <= 100U)
      {
        ticks += step;
        ecl_timer_tick(step);
      }

      std::vector<uint64_t> compare1 = { 6, 42, 79 };
      std::vector<uint64_t> compare2 = { 6, 28, 51, 74, 97 };
      std::vector<uint64_t> compare3 = { 16, 27, 38, 49, 60, 71, 82, 93 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
      CHECK_ARRAY_EQUAL(compare3.data(), callback_list3.data(), compare3.size());
    }

    //=========================================================================
    TEST(ecl_timer_one_shot_big_step_short_delay_insert)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 15, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id2 = ecl_timer_register(callback2,  5, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 11;

      ticks += step;
      ecl_timer_tick(step);

      ticks += step;
      ecl_timer_tick(step);

      std::vector<uint64_t> compare1 = { 22 };
      std::vector<uint64_t> compare2 = { 11, 11, 22, 22 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
      CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), compare2.size());
    }

    //=========================================================================
    TEST(ecl_timer_one_shot_empty_list_huge_tick_before_insert)
    {
      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 5, ECL_TIMER_SINGLE_SHOT);

      callback_list1.clear();

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);

      ecl_timer_enable(ECL_TIMER_ENABLED);

      ticks = 0;

      const uint32_t step = 5;

      for (uint32_t i = 0; i < step; ++i)
      {
        ++ticks;
        ecl_timer_tick(1);
      }

      // Huge tick count.
      ecl_timer_tick(UINT32_MAX - step + 1);

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);

      for (uint32_t i = 0; i < step; ++i)
      {
        ++ticks;
        ecl_timer_tick(1);
      }
      std::vector<uint64_t> compare1 = { 5, 10 };

      CHECK(callback_list1.size() != 0);

      CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), compare1.size());
    }

    //=========================================================================
#if REALTIME_TEST

  #if defined(ETL_TARGET_OS_WINDOWS) // Only Windows priority is currently supported
    #define RAISE_THREAD_PRIORITY  SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST)
    #define FIX_PROCESSOR_AFFINITY SetThreadAffinityMask(GetCurrentThread(), 1);
  #else
    #error No thread priority modifier defined
  #endif

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
        
        if (ecl_timer_tick(tick))
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

    TEST(ecl_timer_threads)
    {
      FIX_PROCESSOR_AFFINITY;

      ecl_timer_init(timers, NTIMERS);

      ecl_timer_id_t id1 = ecl_timer_register(callback1, 400, ECL_TIMER_SINGLE_SHOT);
      ecl_timer_id_t id2 = ecl_timer_register(callback2, 100, ECL_TIMER_REPEATING);
      ecl_timer_id_t id3 = ecl_timer_register(callback3,  10, ECL_TIMER_REPEATING);

      callback_list1.clear();
      callback_list2.clear();
      callback_list3.clear();

      ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
      ecl_timer_start(id2, ECL_TIMER_START_DELAYED);
      
      ecl_timer_enable(ECL_TIMER_ENABLED);

      std::thread t1(timer_event);
      
      bool restart_1 = true;

      while (ticks <= 1000U)
      {
        if ((ticks > 200U) && (ticks < 500U))
        {
          ecl_timer_stop(id3);
        }

        if ((ticks > 600U) && (ticks < 800U))
        {
          ecl_timer_start(id3, ECL_TIMER_START_DELAYED);
        }

        if ((ticks > 500U) && restart_1)
        {
          ecl_timer_start(id1, ECL_TIMER_START_DELAYED);
          restart_1 = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }

      //Join the thread with the main thread
      t1.join();

      CHECK_EQUAL(2U,  callback_list1.size());
      CHECK_EQUAL(10U, callback_list2.size());
      CHECK(callback_list3.size() < 65U);

      std::vector<uint64_t> compare1 = { 400, 900 };
      std::vector<uint64_t> compare2 = { 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 };

      CHECK(callback_list1.size() != 0);
      CHECK(callback_list2.size() != 0);
      CHECK(callback_list3.size() != 0);

      //CHECK_ARRAY_EQUAL(compare1.data(), callback_list1.data(), min(compare1.size(), callback_list1.size()));
      //CHECK_ARRAY_EQUAL(compare2.data(), callback_list2.data(), min(compare2.size(), callback_list2.size()));
    }
#endif
  };
}
