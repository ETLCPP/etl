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
    TEST(test_debounce_value)
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

      // Stable.
      CHECK_FALSE(value.update(3));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());
    }

    //*************************************************************************
    TEST(test_debounce_value_modify_stable_count)
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
      // Change the stable count.
      value.set_stable_count(2);

      //*******************************
      // Transition from unstable to stable.
      // Unstable.
      CHECK_FALSE(value.update(3));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      // Stable & changed.
      CHECK_TRUE(value.update(3));
      CHECK_TRUE(value.is_stable());
      CHECK_TRUE(value.has_changed());
      CHECK_EQUAL(3, value.get());

      // Stable.
      CHECK_FALSE(value.update(3));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());
    }

    //*************************************************************************
    TEST(test_debounce_values_from_an_input_stream)
    {
      std::vector<uint8_t> input = {1, 2, 3, 3, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 6};
      std::vector<uint8_t> output;
      std::vector<uint8_t> expected = {2, 3, 4, 6};

      const uint8_t                         Stable_Count = 3;
      etl::debounce_value<uint8_t, uint8_t> value(0, Stable_Count);

      for (uint8_t v : input)
      {
        if (value.update(v))
        {
          output.push_back(value.get());
        }
      }

      CHECK_EQUAL(expected.size(), output.size());
      CHECK_ARRAY_EQUAL(expected.data(), output.data(), expected.size());
    }
  }
} // namespace
