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

namespace
{
  uint8_t notification_value = 0;

  class Notification : public etl::debounce_value<uint8_t, uint8_t>::notification_type
  {
    void on_debounce_value_notification(uint8_t value) override
    {
      notification_value = value;
    }
  };

  Notification notification;

  SUITE(test_debounce)
  {
    TEST(test_debounce_value_runtime)
    {
      const uint8_t Initial_Value = 1;
      const uint8_t Stable_Count  = 4;

      etl::debounce_value<uint8_t, uint8_t> value(Initial_Value, Stable_Count);

      value.set_notification(notification);

      notification_value = 0;

      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      //*******************************
      // Transition from unstable to stable.
      CHECK_FALSE(value.add(2));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.add(2));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.add(2));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      // Not notified yet.
      CHECK_EQUAL(0, notification_value);

      // Stable & changed.
      CHECK_TRUE(value.add(2));
      CHECK_TRUE(value.is_stable());
      CHECK_TRUE(value.has_changed());

      // Notified.
      CHECK_EQUAL(2, notification_value);

      // Still stable.
      CHECK_TRUE(value.add(2));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      //*******************************
      // Transition from unstable to stable.
      // Unstable.
      CHECK_FALSE(value.add(3));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.add(3));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      CHECK_FALSE(value.add(3));
      CHECK_FALSE(value.is_stable());
      CHECK_FALSE(value.has_changed());

      // Not notified yet.
      CHECK_EQUAL(2, notification_value);

      // Stable & changed.
      CHECK_TRUE(value.add(3));
      CHECK_TRUE(value.is_stable());
      CHECK_TRUE(value.has_changed());

      // Notified.
      CHECK_EQUAL(3, notification_value);

      // Stable.
      CHECK_TRUE(value.add(3));
      CHECK_TRUE(value.is_stable());
      CHECK_FALSE(value.has_changed());
    }
  }
} // namespace
