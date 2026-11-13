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

#include "etl/bitset.h"
#include "etl/debounce_multi.h"

#include <array>
#include <vector>

namespace
{
  using value_type = uint8_t;
  using count_type = uint32_t;

  const value_type Initial_Value = 0;

  const count_type Valid_Count  = 4;
  const count_type Hold_Count   = 3;
  const count_type Repeat_Count = 2;

  const value_type Bit0 = 0;
  const value_type Bit1 = 1;
  const value_type Bit2 = 2;

  const value_type Active0   = 1;
  const value_type Inactive0 = 0;

  const size_t NBits = 3;

  const value_type All_Bits_Inactive = 0;
  const value_type Bit0_Active       = 1;

  const etl::bit_polarity::type Polarity = etl::bit_polarity::Active_High;

  using debounce_valid             = etl::debounce_multi<value_type, NBits, Polarity, Valid_Count>;
  using debounce_valid_hold        = etl::debounce_multi<value_type, NBits, Polarity, Valid_Count, Hold_Count>;
  using debounce_valid_hold_repeat = etl::debounce_multi<value_type, NBits, Polarity, Valid_Count, Hold_Count, Repeat_Count>;
  using debounce_valid_repeat      = etl::debounce_multi<value_type, NBits, Polarity, Valid_Count, 0, Repeat_Count>;

  SUITE(test_debounce_multi_active_high)
  {
    //*************************************************************************
    TEST(test_debounce_status)
    {
      debounce_valid_hold_repeat debouncer;

      CHECK_EQUAL(NBits, debouncer.get_width());
      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));
      CHECK_EQUAL(debounce_valid_hold_repeat::state_id::Off, debouncer.get_state(0));
      CHECK_EQUAL(debounce_valid_hold_repeat::state_id::Off, debouncer.get_state(1));
      CHECK_EQUAL(debounce_valid_hold_repeat::state_id::Off, debouncer.get_state(2));
    }

    //*************************************************************************
    TEST(test_debounce_valid)
    {
      debounce_valid debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      CHECK_EQUAL(int(Bit0_Active), int(debouncer.get_value()));

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_incremental)
    {
      debounce_valid debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      CHECK_EQUAL(int(Bit0_Active), int(debouncer.get_value()));

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }

        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold)
    {
      debounce_valid_hold debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      CHECK_EQUAL(int(Bit0_Active), int(debouncer.get_value()));

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count + Hold_Count; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_TRUE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_incremental)
    {
      debounce_valid_hold debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Ensure no more transitions.
      for (size_t i = 0; i < Valid_Count + Hold_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }

        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_TRUE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }
    }

    //*************************************************************************
    TEST(test_debounce_valid_repeat)
    {
      debounce_valid_repeat debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      CHECK_EQUAL(int(Bit0_Active), int(debouncer.get_value()));

      //*******************************
      // Transition from Valid to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));

      //*******************************
      // Check for more repeat transitions.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_TRUE(debouncer.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_repeat_incremental)
    {
      debounce_valid_repeat debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Valid to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));

      //*******************************
      // Check for more repeat transitions.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_TRUE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat)
    {
      debounce_valid_hold_repeat debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      CHECK_EQUAL(int(Bit0_Active), int(debouncer.get_value()));

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_TRUE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));

      //*******************************
      // Check for more repeat transitions.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        CHECK_FALSE(debouncer.update(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_TRUE(debouncer.is_held(Bit0));
        CHECK_TRUE(debouncer.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(debouncer.update(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_incremental)
    {
      debounce_valid_hold_repeat debouncer(Initial_Value);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_FALSE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_FALSE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_TRUE(debouncer.is_held(Bit0));
        CHECK_FALSE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));

      //*******************************
      // Check for more repeat transitions.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
        {
          CHECK_FALSE(debouncer.update_incremental(Active0));
        }

        CHECK_TRUE(debouncer.update_incremental(Active0));
        CHECK_FALSE(debouncer.has_changed(Bit0));
        CHECK_TRUE(debouncer.is_set(Bit0));
        CHECK_TRUE(debouncer.is_held(Bit0));
        CHECK_TRUE(debouncer.is_repeating(Bit0));
      }

      // Final incremental update.
      // Changed.
      for (size_t j = 0; j < debouncer.get_width() - 1; ++j)
      {
        CHECK_FALSE(debouncer.update_incremental(Active0));
      }

      CHECK_TRUE(debouncer.update_incremental(Active0));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_check_return_to_off)
    {
      debounce_valid_hold_repeat debouncer(Initial_Value);

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Active0);
      }

      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from On to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Inactive0);
      }

      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Held.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Active0);
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        debouncer.update(Active0);
      }

      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Inactive0);
      }

      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Repeating.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Active0);
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        debouncer.update(Active0);
      }

      for (size_t i = 0; i < Repeat_Count; ++i)
      {
        debouncer.update(Active0);
      }

      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Repeating to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Inactive0);
      }

      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_check_return_to_off_incremental)
    {
      debounce_valid_hold_repeat debouncer(Initial_Value);

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }
      }

      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from On to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Inactive0);
        }
      }

      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Held.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }
      }

      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Held to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Inactive0);
        }
      }

      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to Repeating.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }
      }

      for (size_t i = 0; i < Hold_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }
      }

      for (size_t i = 0; i < Repeat_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Active0);
        }
      }

      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_TRUE(debouncer.is_held(Bit0));
      CHECK_TRUE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Repeating to Off.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        for (size_t j = 0; j < debouncer.get_width(); ++j)
        {
          debouncer.update_incremental(Inactive0);
        }
      }

      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(test_debounce_valid_hold_repeat_then_reset)
    {
      debounce_valid_hold_repeat debouncer(Initial_Value);

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Active0);
      }

      //*******************************
      // Transition from On to Held.
      for (size_t i = 0; i < Hold_Count; ++i)
      {
        debouncer.update(Active0);
      }

      //*******************************
      // Partly transition from Held to Repeating.
      for (size_t i = 0; i < Repeat_Count - 1; ++i)
      {
        debouncer.update(Active0);
      }

      debouncer.reset(Initial_Value);

      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      for (size_t i = 0; i < Valid_Count; ++i)
      {
        debouncer.update(Active0);
      }

      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
    }

    //*************************************************************************
    TEST(debounce_valid_hold_repeat_multiple_bits)
    {
      // Input sequence for 3 bits.
      std::array<uint8_t, 45> input = {1, 0, 1, 0, 1, 3, 1, 3, 1, 3, 3, 3, 3, 7, 3, 6, 3, 6, 7, 6, 4, 6, 4,
                                       7, 5, 4, 5, 5, 1, 5, 1, 5, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0};

      using state_id_type = debounce_valid_hold_repeat::state_id::type;
      using state_id      = debounce_valid_hold_repeat::state_id;
      using value_type    = debounce_valid_hold_repeat::value_type;

      std::array<value_type, 45> expected_changed = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 3, 0, 1, 2, 0, 2, 0, 2, 4, 0, 1,
                                                     4, 0, 4, 0, 6, 0, 1, 0, 0, 1, 0, 1, 4, 1, 0, 0, 0, 0, 0, 0, 0, 1};

      std::array<state_id_type, 45> expected_state0 = {
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::On,        state_id::On,        state_id::On,        state_id::Held,      state_id::Held,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::On,
        state_id::On,        state_id::On,        state_id::Held,      state_id::Held,      state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Off};

      std::array<state_id_type, 45> expected_state1 = {
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::On,        state_id::On,        state_id::On,        state_id::Held,      state_id::Held,      state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off};

      std::array<state_id_type, 45> expected_state2 = {
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::On,        state_id::On,        state_id::On,        state_id::Held,
        state_id::Held,      state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating,
        state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Repeating, state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,       state_id::Off,
        state_id::Off,       state_id::Off,       state_id::Off};

      std::array<value_type, 45> expected_value = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 7, 7, 6,
                                                   6, 6, 6, 6, 4, 4, 5, 5, 5, 5, 5, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};

      debounce_valid_hold_repeat debouncer;

      for (size_t i = 0; i < input.size(); ++i)
      {
        debouncer.update(input[i]);

        etl::bitset<8> expected_changed_bits(expected_changed[i]);

        CHECK_EQUAL(int(expected_state0[i]), int(debouncer.get_state(0)));
        CHECK_EQUAL(int(expected_state1[i]), int(debouncer.get_state(1)));
        CHECK_EQUAL(int(expected_state2[i]), int(debouncer.get_state(2)));

        CHECK_EQUAL(int(expected_value[i]), int(debouncer.get_value()));
        CHECK_EQUAL(int(expected_changed[i]), int(debouncer.get_changed()));
      }
    }

    //*************************************************************************
    TEST(test_debounce_reset_bit)
    {
      debounce_valid debouncer(Initial_Value);

      //*******************************
      // Partial transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        debouncer.update(1);
        debouncer.has_changed(Bit0);
        debouncer.is_set(Bit0);
        debouncer.is_held(Bit0);
        debouncer.is_repeating(Bit0);
      }

      // Reset the bit before it has changed.
      debouncer.reset_bit(Bit0);

      CHECK_FALSE(debouncer.has_changed(Bit0));
      CHECK_FALSE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));
      CHECK_EQUAL(int(All_Bits_Inactive), int(debouncer.get_value()));

      // Partial transition from Off to On.
      for (size_t i = 0; i < Valid_Count - 1; ++i)
      {
        debouncer.update(1);
        debouncer.has_changed(Bit0);
        debouncer.is_set(Bit0);
        debouncer.is_held(Bit0);
        debouncer.is_repeating(Bit0);
      }

      // Changed.
      CHECK_TRUE(debouncer.update(1));
      CHECK_TRUE(debouncer.has_changed(Bit0));
      CHECK_TRUE(debouncer.is_set(Bit0));
      CHECK_FALSE(debouncer.is_held(Bit0));
      CHECK_FALSE(debouncer.is_repeating(Bit0));

      CHECK_EQUAL(int(Bit0_Active), int(debouncer.get_value()));
    }
  }
} // namespace
