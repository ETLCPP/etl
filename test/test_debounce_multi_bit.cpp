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

  using debounce_valid             = etl::debounce_multi_bit<value_type, count_type, 1, Valid_Count>;
  using debounce_valid_hold        = etl::debounce_multi_bit<value_type, count_type, 1, Valid_Count, Hold_Count>;
  using debounce_valid_hold_repeat = etl::debounce_multi_bit<value_type, count_type, 1, Valid_Count, Hold_Count, Repeat_Count>;

  SUITE(test_debounce_multi_bit)
  {
    //*************************************************************************
    TEST(test_debounce_no_hold_or_repeat)
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
        CHECK_FALSE(value.update(1));
        CHECK_FALSE(value.has_changed(Bit0));
        CHECK_FALSE(value.is_set(Bit0));
        CHECK_FALSE(value.is_held(Bit0));
        CHECK_FALSE(value.is_repeating(Bit0));
      }

      // Changed.
      CHECK_TRUE(value.update(1));
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

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
  }
} // namespace
