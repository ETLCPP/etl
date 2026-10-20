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

#ifndef ETL_TIMER_DATA_INCLUDED
#define ETL_TIMER_DATA_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "timer.h"

#include <stdint.h>

//*****************************************************************************
// Definitions common to intrusive timers.
//*****************************************************************************

namespace etl
{
  //***************************************************************************
  /// Common definitions for the intrusive timer framework.
  //***************************************************************************
  class timer_config
  {
  public:

    friend class timer_register_list;
    friend class timer_active_list;
    friend class timer_triggered_list;
    friend class intrusive_callback_timer;

    //*******************************************
    timer_config() ETL_NOEXCEPT
      : period(etl::timer::state::Inactive)
      , delta(etl::timer::state::Inactive)
      , mode(etl::timer::mode::Repeating)
      , registered_next(ETL_NULLPTR)
      , active_next(ETL_NULLPTR)
      , triggered_next(ETL_NULLPTR)
    {
    }

    //*******************************************
    timer_config(uint32_t period_, etl::timer::mode::type repeating_) ETL_NOEXCEPT
      : period(period_)
      , delta(period_)
      , mode(repeating_)
      , registered_next(ETL_NULLPTR)
      , active_next(ETL_NULLPTR)
      , triggered_next(ETL_NULLPTR)
    {
    }

    //*******************************************
    uint32_t get_period() const ETL_NOEXCEPT
    {
      return period;
    }

    //*******************************************
    uint32_t get_delta() const ETL_NOEXCEPT
    {
      return delta;
    }

    //*******************************************
    uint32_t get_mode() const ETL_NOEXCEPT
    {
      return mode;
    }

    //*******************************************
    void clear() ETL_NOEXCEPT
    {
      period          = etl::timer::state::Inactive;
      delta           = etl::timer::state::Inactive;
      mode            = etl::timer::mode::Repeating;
      registered_next = ETL_NULLPTR;
      active_next     = ETL_NULLPTR;
      triggered_next  = ETL_NULLPTR;
    }

  private:

    uint32_t               period;          ///< The period of the timer.
    uint32_t               delta;           ///< The time remaining until the timer expires.
    timer_config*          registered_next; ///< The next timer in the registered timer list. nullptr if the timer is not registered.
    timer_config*          active_next;     ///< The next timer in the active timer list. nullptr if the timer is not active.
    timer_config*          triggered_next;  ///< The next timer in the triggered timer list. nullptr if the timer is not triggered.
    etl::timer::mode::type mode;            ///< Whether the timer is repeating or single-shot.
  };

  //***************************************************************************
  /// A timer data structure that includes a callback delegate.
  //***************************************************************************
  template <typename TCallback>
  struct callback_timer_config : public timer_config
  {
    callback_timer_config(etl::timer::id::type id_, TCallback callback_) ETL_NOEXCEPT
      : timer_config()
      , callback(callback_)
    {
    }

    TCallback callback; ///< The callback delegate for the timer.
  };

  //*************************************************************************
  /// The list of timers that are registered.
  //*************************************************************************
  class timer_register_list
  {
  public:

    //*******************************
    timer_register_list() ETL_NOEXCEPT
      : head(ETL_NULLPTR)
      , tail(ETL_NULLPTR)
    {
    }

    //*******************************
    bool empty() const
    {
      return head == ETL_NULLPTR;
    }

    //*******************************
    // Adds the timer in to the list.
    //*******************************
    void insert(etl::timer_config* new_timer) ETL_NOEXCEPT
    {
      if (empty())
      {
        head = new_timer;
      }
      else if (head == new_timer)
      {

      }
      else
      {
        // Remove any existing timer.
        etl::timer_config* previous = head;

        // Step through the list until we find the timer to remove, or reach the end of the list.
        while ((previous->registered_next != ETL_NULLPTR) && (previous->registered_next != new_timer))
        {
          previous = previous->registered_next;
        }

        // If we found the timer, remove it from the list.
        if (previous->registered_next == new_timer)
        {
        }

        tail->registered_next = new_timer;
      }

      tail = new_timer;
    }

    //*******************************
    // Remove the timer from the list.
    // Returns the next timer in the list after the removed timer, or nullptr if the removed timer was the last in the list.
    //*******************************
    etl::timer_config* remove(etl::timer_config* timer) ETL_NOEXCEPT
    {
      if (timer == head)
      {
        head = timer->registered_next;

        if (head == ETL_NULLPTR)
        {
          tail = ETL_NULLPTR;
        }

        timer->registered_next = ETL_NULLPTR;

        return head;
      }
      else
      {
        etl::timer_config* previous = head;

        // Step through the list until we find the timer to remove, or reach the end of the list.
        while ((previous->registered_next != ETL_NULLPTR) && (previous->registered_next != timer))
        {
          previous = previous->registered_next;
        }

        // If we found the timer, remove it from the list.
        if (previous->registered_next == timer)
        {
          previous->registered_next = timer->registered_next;

          if (timer == tail)
          {
            tail = previous;
          }

          timer->registered_next = ETL_NULLPTR;

          return previous->registered_next;
        }
        else
        {
          // Timer not found in the list.
          return ETL_NULLPTR;
        }
      }
    }

    //*******************************
    /// Returns the first timer in the list.
    /// If the list is empty, returns nullptr.
    //*******************************
    etl::timer_config* first() const ETL_NOEXCEPT
    {
      return head;
    }

    //*******************************
    /// Returns the next timer in the list after the given timer.
    /// May be nullptr if the given timer is the last in the list.
    /// If the given timer is nullptr, returns nullptr.
    //*******************************
    etl::timer_config* next(etl::timer_config* timer) const ETL_NOEXCEPT
    {
      if (timer != ETL_NULLPTR)
      {
        return timer->registered_next;
      }
      else
      {
        return ETL_NULLPTR;
      }
    }

    //*******************************
    /// Clear the list of entries.
    //*******************************
    void clear()
    {
      etl::timer_config* timer = head;

      while (timer != ETL_NULLPTR)
      {
        timer = pop_front();
      }

      head = ETL_NULLPTR;
      tail = ETL_NULLPTR;
    }

  private:

    //*******************************
    // Removes the timer at the front of the list.
    // Returns the new head of the list, which may be nullptr if the list is now empty.
    //*******************************
    etl::timer_config* pop_front() ETL_NOEXCEPT
    {
      etl::timer_config* timer = head;
      head                     = timer->registered_next;
      timer->registered_next   = ETL_NULLPTR;

      return head;
    }

    etl::timer_config* head;
    etl::timer_config* tail;
  };

  //*************************************************************************
  /// The list of timers that are active.
  //*************************************************************************
  class timer_active_list
  {
  public:

    //*******************************
    timer_active_list() ETL_NOEXCEPT
      : head(ETL_NULLPTR)
    {
    }

    //*******************************
    bool empty() const ETL_NOEXCEPT
    {
      return head == ETL_NULLPTR;
    }

    //*******************************
    // Inserts the timer at the correct delta position
    //*******************************
    void insert(etl::timer_config* new_timer) ETL_NOEXCEPT
    {
      if (empty())
      {
        // No entries yet.
        head                       = new_timer;
        new_timer->active_next = ETL_NULLPTR;
      }
      else if (new_timer->delta < head->delta)
      {
        // Goes before the head.
        head->delta -= new_timer->delta;
        new_timer->active_next = head;
        head                       = new_timer;
      }
      else
      {
        // We already have entries, and new_timer doesn't go before head.
        timer_config* previous_timer = head;
        timer_config* timer          = head->active_next;

        // Adjust the new_timer delta to take account of the head timer's delta.
        new_timer->delta -= previous_timer->delta;

        bool inserted = false;

        while (!inserted && (timer != ETL_NULLPTR))
        {
          // Find the correct place to insert.
          // The list is sorted by delta, so we need to find the first timer with a delta greater than the new timer's delta.
          if (new_timer->delta < timer->delta)
          {
            // Insert after the previous timer.
            previous_timer->active_next = new_timer;
            new_timer->active_next      = timer;

            // Adjust the next delta for the new timer's delta.
            timer->delta -= new_timer->delta;

            inserted = true;
          }
          else
          {
            // Adjust the new_timer delta to take account of the last timer's delta.
            new_timer->delta -= timer->delta;
          }

          // Move to the next timer in the list.
          previous_timer = timer;
          timer          = timer->active_next;
        }

        if (!inserted)
        {
          // Reached the end so attach to the tail.
          previous_timer->active_next = new_timer;
          new_timer->active_next      = ETL_NULLPTR;
        }
      }
    }

    //*******************************
    // Removes the selected timer from the list.
    // No action is taken if the timer is not found.
    // returns the next timer in the list after the removed timer, or nullptr if the removed timer was the last in the list.
    //*******************************
    etl::timer_config* remove(etl::timer_config* timer) ETL_NOEXCEPT
    {
      // Find the timer in the list.
      etl::timer_config* current = head;
      etl::timer_config* last    = ETL_NULLPTR;

      while (current != timer && current != ETL_NULLPTR)
      {
        last    = current;
        current = current->active_next;
      }

      // Not found?
      if (current == ETL_NULLPTR)
      {
        return ETL_NULLPTR;
      }

      etl::timer_config* next = current->active_next;

      // Adjust the next delta.
      if (next != ETL_NULLPTR)
      {
        next->delta += current->delta;
      }

      // Remove it from the list.
      if (last == ETL_NULLPTR)
      {
        head = next;
      }
      else
      {
        last->active_next = next;
      }

      // Clear the next pointer of the removed timer.
      current->active_next = ETL_NULLPTR;

      return next;
    }

    //*******************************
    /// Returns the first timer in the list.
    /// If the list is empty, returns nullptr.
    //*******************************
    etl::timer_config* first() const ETL_NOEXCEPT
    {
      return head;
    }

    //*******************************
    /// Returns the next timer in the list after the given timer.
    /// If the given timer is nullptr, returns nullptr.
    //*******************************
    etl::timer_config* next(etl::timer_config* timer) const ETL_NOEXCEPT
    {
      if (timer != ETL_NULLPTR)
      {
        return timer->active_next;
      }
      else
      {
        return ETL_NULLPTR;
      }
    }

    //*******************************
    /// Clear the list of entries.
    //*******************************
    void clear()
    {
      etl::timer_config* timer = head;

      while (timer != ETL_NULLPTR)
      {
        timer = pop_front();
      }

      head = ETL_NULLPTR;
    }

  private:

    //*******************************
    // Removes the timer at the front of the list.
    // Returns the new head of the list, which may be nullptr if the list is now empty.
    //*******************************
    etl::timer_config* pop_front() ETL_NOEXCEPT
    {
      etl::timer_config* timer = head;
      head                     = timer->active_next;
      timer->active_next   = ETL_NULLPTR;

      return head;
    }

    etl::timer_config* head;
  };

  //*************************************************************************
  /// The list of timers that are registered.
  //*************************************************************************
  class timer_triggered_list
  {
  public:

    //*******************************
    timer_triggered_list() ETL_NOEXCEPT
      : head(ETL_NULLPTR)
      , tail(ETL_NULLPTR)
    {
    }

    //*******************************
    bool empty() const
    {
      return head == ETL_NULLPTR;
    }

    //*******************************
    // Adds the timer in to the list.
    //*******************************
    void insert(etl::timer_config* new_timer) ETL_NOEXCEPT
    {
      new_timer->triggered_next = ETL_NULLPTR;

      if (empty())
      {
        head = new_timer;
      }
      else
      {
        tail->triggered_next = new_timer;
      }

      tail = new_timer;
    }

    //*******************************
    /// Returns the first timer in the list.
    /// If the list is empty, returns nullptr.
    //*******************************
    etl::timer_config* first() const ETL_NOEXCEPT
    {
      return head;
    }

    //*******************************
    /// Returns the next timer in the list after the given timer.
    /// May be nullptr if the given timer is the last in the list.
    /// If the given timer is nullptr, returns nullptr.
    //*******************************
    etl::timer_config* next(etl::timer_config* timer) const ETL_NOEXCEPT
    {
      if (timer != ETL_NULLPTR)
      {
        return timer->triggered_next;
      }
      else
      {
        return ETL_NULLPTR;
      }
    }

    //*******************************
    /// Clear the list of entries.
    //*******************************
    void clear()
    {
      etl::timer_config* timer = head;

      while (timer != ETL_NULLPTR)
      {
        timer = pop_front();
      }

      head = ETL_NULLPTR;
      tail = ETL_NULLPTR;
    }

  private:

    //*******************************
    // Removes the timer at the front of the list.
    // Returns the new head of the list, which may be nullptr if the list is now empty.
    //*******************************
    etl::timer_config* pop_front() ETL_NOEXCEPT
    {
      etl::timer_config* timer = head;
      head                     = timer->triggered_next;
      timer->triggered_next       = ETL_NULLPTR;

      return head;
    }

    etl::timer_config* head;
    etl::timer_config* tail;
  };
} // namespace etl

#endif
