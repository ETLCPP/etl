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

#include "etl/debounce_value.h"

#include <vector>

namespace
{
  SUITE(test_debounce)
  {
    //*************************************************************************
    TEST(test_debounce_value_with_a_stable_count_of_1)
    {
      const uint8_t Initial_Value = 1;
      const uint8_t Stable_Count  = 1;

      etl::debounce_value<uint8_t, uint8_t> value(Initial_Value, Stable_Count);

      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      //*******************************
      // Transition from unstable to stable.
      // Stable & changed.
      CHECK_TRUE(value.update(2));
      CHECK_TRUE(value.is_stable());
      CHECK_TRUE(value.has_changed());
      CHECK_EQUAL(2, value.get());

      // Still stable.
      CHECK_FALSE(value.update(2));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      //*******************************
      // Transition from unstable to stable.
      // Stable & changed.
      CHECK_TRUE(value.update(3));
      CHECK_TRUE(value.is_stable());
      CHECK_TRUE(value.has_changed());
      CHECK_EQUAL(3, value.get());

      // Still stable.
      CHECK_FALSE(value.update(3));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());
    }

    //*************************************************************************
    TEST(test_debounce_value_with_a_stable_count_of_4)
    {
      const uint8_t Initial_Value = 1;
      const uint8_t Stable_Count  = 4;

      etl::debounce_value<uint8_t, uint8_t> value(Initial_Value, Stable_Count);

      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      //*******************************
      // Transition from unstable to stable.
      CHECK_FALSE(value.update(2));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.update(2));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.update(2));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      // Stable & changed.
      CHECK_TRUE(value.update(2));
      CHECK_TRUE(value.is_stable());
      CHECK_TRUE(value.has_changed());
      CHECK_EQUAL(2, value.get());

      // Still stable.
      CHECK_FALSE(value.update(2));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      //*******************************
      // Transition from unstable to stable.
      // Unstable.
      CHECK_FALSE(value.update(3));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.update(3));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.update(3));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      // Stable & changed.
      CHECK_TRUE(value.update(3));
      CHECK_TRUE(value.is_stable());
      CHECK_TRUE(value.has_changed());
      CHECK_EQUAL(3, value.get());

      // Still stable.
      CHECK_FALSE(value.update(3));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());
    }

    //*************************************************************************
    TEST(test_debounce_values_from_an_input_stream)
    {
      std::vector<uint8_t> input = {1, 2, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6};
      std::vector<uint8_t> output;
      std::vector<int>     actual_update_state;
      std::vector<int>     actual_changed_state;
      std::vector<int>     actual_stable_state;
      std::vector<uint8_t> expected_value         = {2, 3, 4, 6};
      std::vector<int>     expected_update_state  = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
      std::vector<int>     expected_changed_state = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
      std::vector<int>     expected_stable_state  = {0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1};

      const uint8_t Stable_Count = 3;

      etl::debounce_value<uint8_t, uint8_t> value(0, Stable_Count);

      for (uint8_t v : input)
      {
        bool changed = value.update(v);

        if (changed)
        {
          output.push_back(value.get());
        }

        actual_update_state.push_back(changed);
        actual_changed_state.push_back(value.has_changed());
        actual_stable_state.push_back(value.is_stable());
      }

      CHECK_EQUAL(expected_value.size(), output.size());
      CHECK_EQUAL(expected_update_state.size(), actual_update_state.size());
      CHECK_EQUAL(expected_changed_state.size(), actual_changed_state.size());
      CHECK_EQUAL(expected_stable_state.size(), actual_stable_state.size());

      CHECK_ARRAY_EQUAL(expected_value.data(), output.data(), expected_value.size());
      CHECK_ARRAY_EQUAL(expected_update_state.data(), actual_update_state.data(), expected_update_state.size());
      CHECK_ARRAY_EQUAL(expected_changed_state.data(), actual_changed_state.data(), expected_changed_state.size());
      CHECK_ARRAY_EQUAL(expected_stable_state.data(), actual_stable_state.data(), expected_stable_state.size());
    }
  }
} // namespace
