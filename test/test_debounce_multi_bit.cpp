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

  using debounce_type1 = etl::debounce_multi_bit<value_type, count_type, 1, Valid_Count>;

  SUITE(test_debounce_multi_bit)
  {
    //*************************************************************************
    TEST(test_debounce1)
    {
      debounce_type1 value(Initial_Value);

      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      //*******************************
      // Transition from Off to On.
      value.update(1);
      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      value.update(1);
      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      value.update(1);
      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_FALSE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      // Stable & changed.
      value.update(1);
      CHECK_TRUE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      // Still stable.
      value.update(1);
      CHECK_FALSE(value.has_changed(Bit0));
      CHECK_TRUE(value.is_set(Bit0));
      CHECK_FALSE(value.is_held(Bit0));
      CHECK_FALSE(value.is_repeating(Bit0));

      ////*******************************
      //// Transition from unstable to stable.
      //// Unstable.
      // CHECK_FALSE(value.add(3));
      // CHECK_FALSE(value.is_stable());
      // CHECK_FALSE(value.has_changed());

      // CHECK_FALSE(value.add(3));
      // CHECK_FALSE(value.is_stable());
      // CHECK_FALSE(value.has_changed());

      // CHECK_FALSE(value.add(3));
      // CHECK_FALSE(value.is_stable());
      // CHECK_FALSE(value.has_changed());

      //// Stable & changed.
      // CHECK_TRUE(value.add(3));
      // CHECK_TRUE(value.is_stable());
      // CHECK_TRUE(value.has_changed());
      // CHECK_EQUAL(3, value.get());

      //// Stable.
      // CHECK_FALSE(value.add(3));
      // CHECK_TRUE(value.is_stable());
      // CHECK_FALSE(value.has_changed());
    }
  }
} // namespace
