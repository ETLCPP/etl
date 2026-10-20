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

#include "etl/timer_config.h"

namespace
{
  SUITE(test_timer_config)
  {
    //*************************************************************************
    TEST(test_register_list)
    {
      etl::timer_config timer1;
      etl::timer_config timer2;
      etl::timer_config timer3;
      etl::timer_config timer4;
      etl::timer_config timer5;

      etl::timer_register_list register_list;

      CHECK_TRUE(register_list.empty());
      CHECK_TRUE(register_list.first() == ETL_NULLPTR);

      // Add the timers to the list.
      register_list.insert(&timer1);
      register_list.insert(&timer2);
      register_list.insert(&timer3);
      register_list.insert(&timer4);
      register_list.insert(&timer5);

      CHECK_FALSE(register_list.empty());
      CHECK_TRUE(register_list.first() == &timer1);

      etl::timer_config* timer = register_list.first();

      // The timers should be in the order they were added.
      CHECK_EQUAL(&timer1, timer);
      timer = register_list.next(timer);
      CHECK_EQUAL(&timer2, timer);
      timer = register_list.next(timer);
      CHECK_EQUAL(&timer3, timer);
      timer = register_list.next(timer);
      CHECK_EQUAL(&timer4, timer);
      timer = register_list.next(timer);
      CHECK_EQUAL(&timer5, timer);
      timer = register_list.next(timer);
      CHECK_EQUAL(ETL_NULLPTR, timer);

      // Remove timer3 from the list.
      register_list.remove(&timer3);

      // Search the list to ensure the other timers are still present.
      bool timer1_found = false;
      bool timer2_found = false;
      bool timer3_found = false;
      bool timer4_found = false;
      bool timer5_found = false;

      timer = register_list.first();

      while (timer != ETL_NULLPTR)
      {
        if (timer == &timer1)
        {
          timer1_found = true;
        }
        else if (timer == &timer2)
        {
          timer2_found = true;
        }
        else if (timer == &timer3)
        {
          timer3_found = true;
        }
        else if (timer == &timer4)
        {
          timer4_found = true;
        }
        else if (timer == &timer5)
        {
          timer5_found = true;
        }

        timer = register_list.next(timer);
      }

      CHECK_TRUE(timer1_found);
      CHECK_TRUE(timer2_found);
      CHECK_FALSE(timer3_found);
      CHECK_TRUE(timer4_found);
      CHECK_TRUE(timer5_found);

      // Clear the list.
      register_list.clear();

      CHECK_TRUE(register_list.empty());
      CHECK_TRUE(register_list.first() == ETL_NULLPTR);
    }

    //*************************************************************************
    TEST(test_active_list)
    {
      etl::timer_config timer1(100, etl::timer::mode::Repeating);
      etl::timer_config timer2(65, etl::timer::mode::Repeating);
      etl::timer_config timer3(85, etl::timer::mode::Repeating);
      etl::timer_config timer4(130, etl::timer::mode::Repeating);
      etl::timer_config timer5(85, etl::timer::mode::Repeating);
      etl::timer_config timer6(85, etl::timer::mode::Repeating);

      etl::timer_active_list active_list;

      CHECK_TRUE(active_list.empty());
      CHECK_TRUE(active_list.first() == ETL_NULLPTR);

      active_list.insert(&timer1);
      active_list.insert(&timer2);
      active_list.insert(&timer3);
      active_list.insert(&timer4);
      active_list.insert(&timer5);

      CHECK_FALSE(active_list.empty());
      CHECK_TRUE(active_list.first() == &timer2);

      etl::timer_config* timer = active_list.first();

      // The timers should be in the order of their deltas.
      CHECK_EQUAL(&timer2, timer);
      timer = active_list.next(timer);
      CHECK_EQUAL(&timer3, timer);
      timer = active_list.next(timer);
      CHECK_EQUAL(&timer5, timer);
      timer = active_list.next(timer);
      CHECK_EQUAL(&timer1, timer);
      timer = active_list.next(timer);
      CHECK_EQUAL(&timer4, timer);
      timer = active_list.next(timer);
      CHECK_EQUAL(ETL_NULLPTR, timer);

      CHECK_EQUAL(timer2.get_period(), timer2.get_delta());
      CHECK_EQUAL(timer3.get_period() - timer2.get_period(), timer3.get_delta());
      CHECK_EQUAL(timer1.get_period() - timer3.get_period(), timer1.get_delta());
      CHECK_EQUAL(timer4.get_period() - timer1.get_period(), timer4.get_delta());
      CHECK_EQUAL(timer5.get_period() - timer3.get_period(), timer5.get_delta());

      // Remove timer3 from the list.
      active_list.remove(&timer3);

      // Search the list to ensure the other timers are still present.
      bool timer1_found = false;
      bool timer2_found = false;
      bool timer3_found = false;
      bool timer4_found = false;
      bool timer5_found = false;

      timer = active_list.first();

      while (timer != ETL_NULLPTR)
      {
        if (timer == &timer1)
        {
          timer1_found = true;
        }
        else if (timer == &timer2)
        {
          timer2_found = true;
        }
        else if (timer == &timer3)
        {
          timer3_found = true;
        }
        else if (timer == &timer4)
        {
          timer4_found = true;
        }
        else if (timer == &timer5)
        {
          timer5_found = true;
        }

        timer = active_list.next(timer);
      }

      CHECK_TRUE(timer1_found);
      CHECK_TRUE(timer2_found);
      CHECK_FALSE(timer3_found);
      CHECK_TRUE(timer4_found);
      CHECK_TRUE(timer5_found);

      // Check the deltas are still correct.
      CHECK_EQUAL(timer2.get_period(), timer2.get_delta());
      CHECK_EQUAL(timer1.get_period() - timer3.get_period(), timer1.get_delta());
      CHECK_EQUAL(timer4.get_period() - timer1.get_period(), timer4.get_delta());
      CHECK_EQUAL(timer5.get_period() - timer2.get_period(), timer5.get_delta());

      // Remove a timer that is not in the list.
      active_list.remove(&timer6);

      // Search the list to ensure the other timers are still present.
      timer1_found = false;
      timer2_found = false;
      timer3_found = false;
      timer4_found = false;
      timer5_found = false;

      timer = active_list.first();

      while (timer != ETL_NULLPTR)
      {
        if (timer == &timer1)
        {
          timer1_found = true;
        }
        else if (timer == &timer2)
        {
          timer2_found = true;
        }
        else if (timer == &timer3)
        {
          timer3_found = true;
        }
        else if (timer == &timer4)
        {
          timer4_found = true;
        }
        else if (timer == &timer5)
        {
          timer5_found = true;
        }

        timer = active_list.next(timer);
      }

      // Check the deltas are still correct.
      CHECK_EQUAL(timer2.get_period(), timer2.get_delta());
      CHECK_EQUAL(timer1.get_period() - timer3.get_period(), timer1.get_delta());
      CHECK_EQUAL(timer4.get_period() - timer1.get_period(), timer4.get_delta());
      CHECK_EQUAL(timer5.get_period() - timer2.get_period(), timer5.get_delta());

      // Clear the list.
      active_list.clear();

      CHECK_TRUE(active_list.empty());
      CHECK_TRUE(active_list.first() == ETL_NULLPTR);
    }

    //*************************************************************************
    TEST(test_triggered_list)
    {
      etl::timer_config timer1;
      etl::timer_config timer2;
      etl::timer_config timer3;
      etl::timer_config timer4;
      etl::timer_config timer5;

      etl::timer_triggered_list triggered_list;

      CHECK_TRUE(triggered_list.empty());
      CHECK_TRUE(triggered_list.first() == ETL_NULLPTR);

      // Add the timers to the list.
      triggered_list.insert(&timer1);
      triggered_list.insert(&timer2);
      triggered_list.insert(&timer3);
      triggered_list.insert(&timer4);
      triggered_list.insert(&timer5);

      CHECK_FALSE(triggered_list.empty());
      CHECK_TRUE(triggered_list.first() == &timer1);

      // The timers should be in the order they were added.
      etl::timer_config* timer = triggered_list.first();

      CHECK_EQUAL(&timer1, timer);
      timer = triggered_list.next(timer);
      CHECK_EQUAL(&timer2, timer);
      timer = triggered_list.next(timer);
      CHECK_EQUAL(&timer3, timer);
      timer = triggered_list.next(timer);
      CHECK_EQUAL(&timer4, timer);
      timer = triggered_list.next(timer);
      CHECK_EQUAL(&timer5, timer);
      timer = triggered_list.next(timer);
      CHECK_EQUAL(ETL_NULLPTR, timer);

      // Clear the list.
      triggered_list.clear();

      CHECK_TRUE(triggered_list.empty());
      CHECK_TRUE(triggered_list.first() == ETL_NULLPTR);
    }
  }
} // namespace
