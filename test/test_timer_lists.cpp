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

#include "etl/timer_node.h"
#include "etl/private/timer_lists.h"

namespace etl_unit_test
{
  //*******************************************
  class etl_unit_test_intrusive_timer
  {
  public:

    //*************************************************************************
    void register_timer(etl::private_timer::register_list& register_list, etl::timer::node& timer_)
    {
      timer_.set_id(get_next_id(register_list));
      register_list.insert(&timer_);
      timer_.set_owner(this);
    }

    //*******************************************
    etl::timer::id::type get_next_id(etl::private_timer::register_list& register_list) ETL_NOEXCEPT
    {
      etl::timer::id::type next_id = 0; // IDs start at 0

      etl::timer::node* timer = register_list.first();

      while (timer != register_list.end())
      {
        if (timer->get_id() == next_id)
        {
          // Already in use, so increment next_id and continue checking
          ++next_id;
        }
        else if (timer->get_id() > next_id)
        {
          // Found a gap in the IDs, so return the next_id
          return next_id;
        }

        timer = register_list.next(timer);
      }

      // We never found a gap, so return the next available ID
      return next_id;
    }
  };
}

namespace
{
  etl_unit_test::etl_unit_test_intrusive_timer test_intrusive_timer;

  SUITE(test_timer_lists)
  {
    //*************************************************************************
    TEST(test_timer_register_list)
    {
      constexpr etl::timer::config timer1_config(100, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer2_config(65, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer3_config(85, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer4_config(130, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer5_config(85, etl::timer::mode::Repeating);
      etl::timer::node             timer1{timer1_config};
      etl::timer::node             timer2{timer2_config};
      etl::timer::node             timer3{timer3_config};
      etl::timer::node             timer4{timer4_config};
      etl::timer::node             timer5{timer5_config};

      etl::private_timer::register_list register_list;

      CHECK_TRUE(register_list.empty());
      CHECK_TRUE(register_list.first() == register_list.end());

      // Add the timers to the list.
      test_intrusive_timer.register_timer(register_list, timer1);
      test_intrusive_timer.register_timer(register_list, timer2);
      test_intrusive_timer.register_timer(register_list, timer3);
      test_intrusive_timer.register_timer(register_list, timer4);
      test_intrusive_timer.register_timer(register_list, timer5);

      CHECK_FALSE(register_list.empty());
      CHECK_TRUE(register_list.first() == &timer1);

      // Remove timer3 from the list.
      register_list.remove(&timer3);

      // Search the list to ensure the other timers are still present.
      bool timer1_found = false;
      bool timer2_found = false;
      bool timer3_found = false;
      bool timer4_found = false;
      bool timer5_found = false;

      etl::timer::node* timer = register_list.first();

      while (timer != register_list.end())
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
      CHECK_TRUE(register_list.first() == register_list.end());
    }

    //*************************************************************************
    TEST(test_timer_active_list)
    {
      constexpr etl::timer::config timer1_config(100, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer2_config(65, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer3_config(85, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer4_config(130, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer5_config(85, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer6_config(85, etl::timer::mode::Repeating);

      etl::timer::node timer1(timer1_config);
      etl::timer::node timer2(timer2_config);
      etl::timer::node timer3(timer3_config);
      etl::timer::node timer4(timer4_config);
      etl::timer::node timer5(timer5_config);
      etl::timer::node timer6(timer6_config);

      etl::private_timer::active_list active_list;

      CHECK_TRUE(active_list.empty());
      CHECK_TRUE(active_list.first() == active_list.end());

      active_list.insert(&timer1);
      active_list.insert(&timer2);
      active_list.insert(&timer3);
      active_list.insert(&timer4);
      active_list.insert(&timer5);

      CHECK_FALSE(active_list.empty());
      CHECK_TRUE(active_list.first() == &timer2);

      etl::timer::node* timer = active_list.first();

      // The timers should be in the order of their intervals.
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
      CHECK_EQUAL(active_list.end(), timer);

      // Check the deltas are correct.
      // The interval order of the timers in the list is timer2, timer3, timer5, timer1, timer4.
      // timer 5 has the same interval as timer 3, so it should be after timer 3 in the list.
      CHECK_EQUAL(timer2.get_interval(), timer2.get_delta());
      CHECK_EQUAL(timer3.get_interval() - timer2.get_interval(), timer3.get_delta());
      CHECK_EQUAL(timer1.get_interval() - timer3.get_interval(), timer1.get_delta());
      CHECK_EQUAL(timer4.get_interval() - timer1.get_interval(), timer4.get_delta());
      CHECK_EQUAL(timer5.get_interval() - timer3.get_interval(), timer5.get_delta());

      // Remove timer3 from the list.
      active_list.remove(&timer3);

      // Check the deltas are still correct.
      // The interval order of the timers in the list is now timer2, timer5, timer1, timer4.
      CHECK_EQUAL(timer2.get_interval(), timer2.get_delta());
      CHECK_EQUAL(timer5.get_interval() - timer2.get_interval(), timer5.get_delta());
      CHECK_EQUAL(timer1.get_interval() - timer5.get_interval(), timer1.get_delta());
      CHECK_EQUAL(timer4.get_interval() - timer1.get_interval(), timer4.get_delta());

      // Search the list to ensure the other timers are still present.
      bool timer1_found = false;
      bool timer2_found = false;
      bool timer3_found = false;
      bool timer4_found = false;
      bool timer5_found = false;

      timer = active_list.first();

      while (timer != active_list.end())
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

      // Clear the list.
      active_list.clear();

      CHECK_TRUE(active_list.empty());
      CHECK_TRUE(active_list.first() == active_list.end());
    }

    //*************************************************************************
    TEST(test_timer_expired_list)
    {
      constexpr etl::timer::config timer1_config(100, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer2_config(65, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer3_config(85, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer4_config(130, etl::timer::mode::Repeating);
      constexpr etl::timer::config timer5_config(85, etl::timer::mode::Repeating);
      etl::timer::node             timer1{timer1_config};
      etl::timer::node             timer2{timer2_config};
      etl::timer::node             timer3{timer3_config};
      etl::timer::node             timer4{timer4_config};
      etl::timer::node             timer5{timer5_config};

      etl::private_timer::expired_list expired_list;

      CHECK_TRUE(expired_list.empty());
      CHECK_TRUE(expired_list.first() == expired_list.end());

      // Add the timers to the list.
      expired_list.insert(&timer1);
      expired_list.insert(&timer2);
      expired_list.insert(&timer3);
      expired_list.insert(&timer4);
      expired_list.insert(&timer5);

      CHECK_FALSE(expired_list.empty());
      CHECK_TRUE(expired_list.first() == &timer1);

      // The timers should be in the order they were added.
      etl::timer::node* timer = expired_list.first();

      CHECK_EQUAL(&timer1, timer);
      timer = expired_list.next(timer);
      CHECK_EQUAL(&timer2, timer);
      timer = expired_list.next(timer);
      CHECK_EQUAL(&timer3, timer);
      timer = expired_list.next(timer);
      CHECK_EQUAL(&timer4, timer);
      timer = expired_list.next(timer);
      CHECK_EQUAL(&timer5, timer);
      timer = expired_list.next(timer);
      CHECK_EQUAL(expired_list.end(), timer);

      // Clear the list.
      expired_list.clear();

      CHECK_TRUE(expired_list.empty());
      CHECK_TRUE(expired_list.first() == expired_list.end());
    }
  }
} // namespace
