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

#include "etl/debounce_multi_bit.h"

#include <array>
#include <vector>

namespace
{
  using value_type = uint8_t;
  using count_type = uint16_t;

  const value_type Initial_Value = 0;

  const count_type Valid_Count  = 4;
  const count_type Hold_Count   = 3;
  const count_type Repeat_Count = 2;

  const value_type Bit0 = 0;
  const value_type Bit1 = 1;
  const value_type Bit2 = 2;

  const size_t NBits = 3;

  using debounce_valid             = etl::debounce_multi_bit<value_type, count_type, NBits, Valid_Count>;
  using debounce_valid_hold        = etl::debounce_multi_bit<value_type, count_type, NBits, Valid_Count, Hold_Count>;
  using debounce_valid_hold_repeat = etl::debounce_multi_bit<value_type, count_type, NBits, Valid_Count, Hold_Count, Repeat_Count>;

  SUITE(test_debounce_multi_bit)
  {
    //*************************************************************************
    TEST(test_debounce_status)
    {
      debounce_valid_hold_repeat value;

      CHECK_EQUAL(NBits, value.get_width());
      CHECK_EQUAL(0, value.get_state());
      CHECK_EQUAL(debounce_valid_hold_repeat::state_id::Off, value.get_state(0));
      CHECK_EQUAL(debounce_valid_hold_repeat::state_id::Off, value.get_state(1));
      CHECK_EQUAL(debounce_valid_hold_repeat::state_id::Off, value.get_state(2));
    }

    //*************************************************************************
    TEST(test_debounce_valid)
    {
      debounce_valid value(Initial_Value);

      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));
      CHECK_EQUAL(0, value.get_state());

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_FALSE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      CHECK_EQUAL(0, value.get_state());

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      CHECK_EQUAL(1, value.get_state());

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_incremental)
    {
      debounce_valid value(Initial_Value);

      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        for (size_t j = 0; j < value.get_width() - 1; ++j)
        {
          CHECK_FALSE(value.update_incremental(1));
        }

        CHECK_TRUE(value.update_incremental(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_FALSE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      CHECK_EQUAL(0, value.get_state());

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < value.get_width() - 1; ++j)
      {
        CHECK_FALSE(value.update_incremental(1));
      }

      CHECK_TRUE(value.update_incremental(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      CHECK_EQUAL(1, value.get_state());

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }

        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold)
    {
      debounce_valid_hold value(Initial_Value);

      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));
      CHECK_EQUAL(0, value.get_state());

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_FALSE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      CHECK_EQUAL(0, value.get_state());

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      CHECK_EQUAL(1, value.get_state());

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count + Hold_Count; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_TRUE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_incremental)
    {
      debounce_valid_hold value(Initial_Value);

      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        for (size_t j = 0; j < value.get_width() - 1; ++j)
        {
          CHECK_FALSE(value.update_incremental(1));
        }

        CHECK_TRUE(value.update_incremental(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_FALSE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < value.get_width() - 1; ++j)
      {
        CHECK_FALSE(value.update_incremental(1));
      }

      CHECK_TRUE(value.update_incremental(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        for (size_t j = 0; j < value.get_width() - 1; ++j)
        {
          CHECK_FALSE(value.update_incremental(1));
        }

        CHECK_TRUE(value.update_incremental(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < value.get_width() - 1; ++j)
      {
        CHECK_FALSE(value.update_incremental(1));
      }

      CHECK_TRUE(value.update_incremental(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count + Hold_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }

        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_TRUE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat)
    {
      debounce_valid_hold_repeat value(Initial_Value);

      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));
      CHECK_EQUAL(0, value.get_state());

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_FALSE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      CHECK_EQUAL(0, value.get_state());

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      CHECK_EQUAL(1, value.get_state());

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_TRUE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_TRUE(value.is_repeating(Bit0));

      //*******************************
      // Check for more repeat transitions.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_TRUE(value.is_held(Bit0));
        CHECK_TRUE(value.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_TRUE(value.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_incremental)
    {
      debounce_valid_hold_repeat value(Initial_Value);

      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        for (size_t j = 0; j < value.get_width() - 1; ++j)
        {
          CHECK_FALSE(value.update_incremental(1));
        }

        CHECK_TRUE(value.update_incremental(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_FALSE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < value.get_width() - 1; ++j)
      {
        CHECK_FALSE(value.update_incremental(1));
      }

      CHECK_TRUE(value.update_incremental(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        for (size_t j = 0; j < value.get_width() - 1; ++j)
        {
          CHECK_FALSE(value.update_incremental(1));
        }

        CHECK_TRUE(value.update_incremental(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < value.get_width() - 1; ++j)
      {
        CHECK_FALSE(value.update_incremental(1));
      }

      CHECK_TRUE(value.update_incremental(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        for (size_t j = 0; j < value.get_width() - 1; ++j)
        {
          CHECK_FALSE(value.update_incremental(1));
        }

        CHECK_TRUE(value.update_incremental(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_TRUE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < value.get_width() - 1; ++j)
      {
        CHECK_FALSE(value.update_incremental(1));
      }

      CHECK_TRUE(value.update_incremental(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_TRUE(value.is_repeating(Bit0));

      //*******************************
      // Check for more repeat transitions.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        for (size_t j = 0; j < value.get_width() - 1; ++j)
        {
          CHECK_FALSE(value.update_incremental(1));
        }

        CHECK_TRUE(value.update_incremental(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_TRUE(value.is_set(Bit0));
        CHECK_TRUE(value.is_held(Bit0));
        CHECK_TRUE(value.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < value.get_width() - 1; ++j)
      {
        CHECK_FALSE(value.update_incremental(1));
      }

      CHECK_TRUE(value.update_incremental(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_TRUE(value.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_check_return_to_off)
    {
      debounce_valid_hold_repeat value(Initial_Value);

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(1);
      }

      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from On to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(0);
      }

      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Held.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(1);
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        value.update(1);
      }

      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(0);
      }

      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Repeating.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(1);
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        value.update(1);
      }

      for (size_t i = 0; i < Repeat_Count; ++i)
      {
        value.update(1);
      }

      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_TRUE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Repeating to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(0);
      }

      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_check_return_to_off_incremental)
    {
      debounce_valid_hold_repeat value(Initial_Value);

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }
      }

      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from On to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(0);
        }
      }

      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Held.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }
      }

      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(0);
        }
      }

      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Repeating.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }
      }

      for (size_t i = 0; i < Repeat_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(1);
        }
      }

      CHECK_TRUE(value.is_set(Bit0));
      CHECK_TRUE(value.is_held(Bit0));
      CHECK_TRUE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Repeating to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < value.get_width(); ++j)
        {
          value.update_incremental(0);
        }
      }

      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_then_reset)
    {
      debounce_valid_hold_repeat value(Initial_Value);

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(1);
      }

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count; ++i)
      {
        value.update(1);
      }

      //*******************************
      // Partly transition from Held to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        value.update(1);
      }

      value.reset(Initial_Value);

      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        value.update(1);
      }

      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(debounce_valid_hold_repeat_multiple_bits)
    {
      std::array<uint8_t, 45> input = {1, 0, 1, 0, 1, 3, 1, 3, 1, 3, 3, 3, 3, 7, 3, 6, 3, 6, 7, 6, 4, 6, 4,
                                       7, 5, 4, 5, 5, 1, 5, 1, 5, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0};

      using state_id_type = debounce_valid_hold_repeat::state_id_type;
      using state_id      = debounce_valid_hold_repeat::state_id;

      std::array<state_id_type, 45> expected0 = {
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::On,        state_id::On,        state_id::On,        state_id::Held,      state_id::Held,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::On,
        state_id::On,        state_id::On,        state_id::Held,      state_id::Held,      state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Off};

      std::array<state_id_type, 45> expected1 = {
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::On,        state_id::On,        state_id::On,        state_id::Held,      state_id::Held,      state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off};

      std::array<state_id_type, 45> expected2 = {
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::On,        state_id::On,        state_id::On,        state_id::Held,
        state_id::Held,      state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off};

      debounce_valid_hold_repeat value;

      for (size_t i = 0; i < input.size(); ++i)
      {
        value.update(input[i]);

        CHECK_EQUAL(int(expected0[i]), int(value.get_state(0)));
        CHECK_EQUAL(int(expected1[i]), int(value.get_state(1)));
        CHECK_EQUAL(int(expected2[i]), int(value.get_state(2)));
      }
    }
  }
} // namespace
