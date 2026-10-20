/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_CALLBACK_TIMER_INCLUDED
#define ETL_CALLBACK_TIMER_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "atomic.h"
#include "delegate.h"
#include "error_handler.h"
#include "function.h"
#include "nullptr.h"
#include "placement_new.h"
#include "static_assert.h"
#include "timer_data.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Interface for callback timer
  //***************************************************************************
  class iintrusive_callback_timer
  {
  public:

    typedef etl::delegate<void(void)> callback_type;

    typedef etl::delegate<void(etl::timer_data::id::type, uint32_t delta)> event_callback_type;

    //*******************************************
    /// Register a timer.
    //*******************************************
    etl::timer::id::type register_timer(void (*p_callback_)(), uint32_t period_, bool repeating_)
    {
      return id;
    }

    //*******************************************
    /// Register a timer.
    //*******************************************
    etl::timer::id::type register_timer(etl::ifunction<void>& callback_, uint32_t period_, bool repeating_)
    {
      etl::timer::id::type id = etl::timer::id::NO_TIMER;

      bool is_space = (registered_timers < MAX_TIMERS);

      if (is_space)
      {
        // Search for the free space.
        for (uint_least8_t i = 0U; i < MAX_TIMERS; ++i)
        {
          timer_data& timer = timer_array[i];

          if (timer.id == etl::timer::id::NO_TIMER)
          {
            // Create in-place.
            new (&timer) timer_data(i, callback_, period_, repeating_);
            ++registered_timers;
            id = i;
            break;
          }
        }
      }

      return id;
    }

    //*******************************************
    /// Register a timer.
    //*******************************************
#if ETL_USING_CPP11
    etl::timer::id::type register_timer(callback_type& callback_, uint32_t period_, bool repeating_)
    {
      etl::timer::id::type id = etl::timer::id::NO_TIMER;

      bool is_space = (registered_timers < MAX_TIMERS);

      if (is_space)
      {
        // Search for the free space.
        for (uint_least8_t i = 0U; i < MAX_TIMERS; ++i)
        {
          timer_data& timer = timer_array[i];

          if (timer.id == etl::timer::id::NO_TIMER)
          {
            // Create in-place.
            new (&timer) timer_data(i, callback_, period_, repeating_);
            ++registered_timers;
            id = i;
            break;
          }
        }
      }

      return id;
    }
#endif

    //*******************************************
    /// Unregister a timer.
    //*******************************************
    bool unregister_timer(etl::timer::id::type id_)
    {
      bool result = false;

      if (id_ != etl::timer::id::NO_TIMER)
      {
        timer_data& timer = timer_array[id_];

        if (timer.id != etl::timer::id::NO_TIMER)
        {
          if (timer.is_active())
          {
            ETL_DISABLE_TIMER_UPDATES;
            active_list.remove(timer.id, false);
            remove_callback.call_if(timer.id);
            ETL_ENABLE_TIMER_UPDATES;
          }

          // Reset in-place.
          new (&timer) timer_data();
          --registered_timers;

          result = true;
        }
      }

      return result;
    }

    //*******************************************
    /// Enable/disable the timer.
    //*******************************************
    void enable(bool state_)
    {
      enabled = state_;
    }

    //*******************************************
    /// Get the enable/disable state.
    //*******************************************
    bool is_running() const
    {
      return enabled;
    }

    //*******************************************
    /// Clears the timer of data.
    //*******************************************
    void clear()
    {
      ETL_DISABLE_TIMER_UPDATES;
      active_list.clear();
      ETL_ENABLE_TIMER_UPDATES;

      for (int i = 0; i < MAX_TIMERS; ++i)
      {
        ::new (&timer_array[i]) timer_data();
      }

      registered_timers = 0;
    }

    //*******************************************
    // Called by the timer service to indicate the
    // amount of time that has elapsed since the last successful call to 'tick'.
    // Returns true if the tick was processed,
    // false if not.
    //*******************************************
    bool tick(uint32_t count)
    {
      if (enabled)
      {
        if (ETL_TIMER_UPDATES_ENABLED)
        {
          // We have something to do?
          bool has_active = !active_list.empty();

          if (has_active)
          {
            while (has_active && (count >= active_list.front().delta))
            {
              timer_data& timer = active_list.front();

              count -= timer.delta;

              active_list.remove(timer.id, true);
              remove_callback.call_if(timer.id);

              if (timer.repeating)
              {
                // Reinsert the timer.
                timer.delta = timer.period;
                active_list.insert(timer.id);
                insert_callback.call_if(timer.id);
              }

              if (timer.p_callback != ETL_NULLPTR)
              {
                if (timer.cbk_type == timer_data::C_CALLBACK)
                {
                  // Call the C callback.
                  reinterpret_cast<void (*)()>(timer.p_callback)();
                }
                else if (timer.cbk_type == timer_data::IFUNCTION)
                {
                  // Call the function wrapper callback.
                  (*reinterpret_cast<etl::ifunction<void>*>(timer.p_callback))();
                }
                else if (timer.cbk_type == timer_data::DELEGATE)
                {
                  // Call the delegate callback.
                  (*reinterpret_cast<callback_type*>(timer.p_callback))();
                }
              }

              has_active = !active_list.empty();
            }

            if (has_active)
            {
              // Subtract any remainder from the next due timeout.
              active_list.front().delta -= count;
            }
          }

          return true;
        }
      }

      return false;
    }

    //*******************************************
    /// Starts a timer.
    //*******************************************
    bool start(etl::timer::id::type id_, bool immediate_ = false)
    {
      bool result = false;

      // Valid timer id?
      if (id_ != etl::timer::id::NO_TIMER)
      {
        timer_data& timer = timer_array[id_];

        // Registered timer?
        if (timer.id != etl::timer::id::NO_TIMER)
        {
          // Has a valid period.
          if (timer.period != etl::timer::state::Inactive)
          {
            ETL_DISABLE_TIMER_UPDATES;
            if (timer.is_active())
            {
              active_list.remove(timer.id, false);
              remove_callback.call_if(timer.id);
            }

            timer.delta = immediate_ ? 0 : timer.period;
            active_list.insert(timer.id);
            insert_callback.call_if(timer.id);
            ETL_ENABLE_TIMER_UPDATES;

            result = true;
          }
        }
      }

      return result;
    }

    //*******************************************
    /// Stops a timer.
    //*******************************************
    bool stop(etl::timer::id::type id_)
    {
      bool result = false;

      // Valid timer id?
      if (id_ != etl::timer::id::NO_TIMER)
      {
        timer_data& timer = timer_array[id_];

        // Registered timer?
        if (timer.id != etl::timer::id::NO_TIMER)
        {
          if (timer.is_active())
          {
            ETL_DISABLE_TIMER_UPDATES;
            active_list.remove(timer.id, false);
            remove_callback.call_if(timer.id);
            ETL_ENABLE_TIMER_UPDATES;
          }

          result = true;
        }
      }

      return result;
    }

    //*******************************************
    /// Sets a timer's period.
    //*******************************************
    bool set_period(etl::timer::id::type id_, uint32_t period_)
    {
      if (stop(id_))
      {
        timer_array[id_].period = period_;
        return true;
      }

      return false;
    }

    //*******************************************
    /// Sets a timer's mode.
    //*******************************************
    bool set_mode(etl::timer::id::type id_, bool repeating_)
    {
      if (stop(id_))
      {
        timer_array[id_].repeating = repeating_;
        return true;
      }

      return false;
    }

    //*******************************************
    /// Returns true if an attempt was ever made to insert a timer into the
    /// active list when it was already there. Such an insert is rejected, as
    /// it would create a circular reference in the list.
    //*******************************************
    bool duplicate_insert_detected() const
    {
      return active_list.duplicate_insert_detected();
    }

    //*******************************************
    /// Check if there is an active timer.
    //*******************************************
    bool has_active_timer() const
    {
      return !active_list.empty();
    }

    //*******************************************
    /// Get the time to the next timer event.
    /// Returns etl::timer::interval::No_Active_Interval if there is no active
    /// timer.
    //*******************************************
    uint32_t time_to_next() const
    {
      uint32_t delta = static_cast<uint32_t>(etl::timer::interval::No_Active_Interval);

      if (has_active_timer())
      {
        delta = active_list.front().delta;
      }

      return delta;
    }

    //*******************************************
    /// Checks if a timer is currently active.
    /// Returns <b>true</b> if the timer is active, otherwise <b>false</b>.
    //*******************************************
    bool is_active(etl::timer::id::type id_) const
    {
      //// Valid timer id?
      // if (is_valid_timer_id(id_))
      //{
      //   if (has_active_timer())
      //   {
      //     const timer_data& timer = timer_array[id_];

      //    // Registered timer?
      //    if (timer.id != etl::timer::id::NO_TIMER)
      //    {
      //      return timer.is_active();
      //    }
      //  }
      //}

      return false;
    }

    ////*******************************************
    ///// Set a callback when a timer is inserted on list
    ////*******************************************
    // void set_insert_callback(event_callback_type insert_)
    //{
    //   insert_callback = insert_;
    // }

    ////*******************************************
    ///// Set a callback when a timer is removed from list
    ////*******************************************
    // void set_remove_callback(event_callback_type remove_)
    //{
    //   remove_callback = remove_;
    // }

    //*******************************************
    void clear_insert_callback()
    {
      insert_callback.clear();
    }

    //*******************************************
    void clear_remove_callback()
    {
      remove_callback.clear();
    }

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    icallback_timer(timer_data* const timer_array_, const uint_least8_t Max_Timers_)
      : timer_array(timer_array_)
      , active_list(timer_array_)
      , enabled(false)
      ,
#if defined(ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK)
      process_semaphore(0)
      ,
#endif
      registered_timers(0)
      , MAX_TIMERS(Max_Timers_)
    {
    }

  private:

    //*******************************************
    /// Check that the timer id is valid.
    //*******************************************
    bool is_valid_timer_id(etl::timer::id::type id_) const
    {
      return (id_ < MAX_TIMERS);
    }

    //*************************************************************************
    class timer_list
    {
    public:

      //*******************************
      timer_list(timer_data* ptimers_)
        : head(etl::timer::id::NO_TIMER)
        , tail(etl::timer::id::NO_TIMER)
        , ptimers(ptimers_)
        , duplicate_insert(false)
      {
      }

      //*******************************
      bool empty() const
      {
        return head == etl::timer::id::NO_TIMER;
      }

      //*******************************
      /// Returns true if an attempt was ever made to insert a timer that was
      /// already in the list.
      //*******************************
      bool duplicate_insert_detected() const
      {
        return duplicate_insert;
      }

      //*******************************
      // Inserts the timer at the correct delta position
      //*******************************
      void insert(etl::timer::id::type id_)
      {
        timer_data& timer = ptimers[id_];

        // A timer is in this list exactly when it is the head or has a previous
        // link, as remove() clears both links. Inserting one that is already
        // here would link it to itself and create a circular reference, so
        // record the attempt and abandon the insert to keep the list intact.
        if ((head == id_) || (timer.previous != etl::timer::id::NO_TIMER))
        {
          duplicate_insert = true;
          return;
        }

        if (head == etl::timer::id::NO_TIMER)
        {
          // No entries yet.
          head           = id_;
          tail           = id_;
          timer.previous = etl::timer::id::NO_TIMER;
          timer.next     = etl::timer::id::NO_TIMER;
        }
        else
        {
          // We already have entries.
          etl::timer::id::type test_id = begin();

          while (test_id != etl::timer::id::NO_TIMER)
          {
            timer_data& test = ptimers[test_id];

            // Find the correct place to insert.
            if (timer.delta <= test.delta)
            {
              if (test.id == head)
              {
                head = timer.id;
              }

              // Insert before test.
              timer.previous = test.previous;
              test.previous  = timer.id;
              timer.next     = test.id;

              // Adjust the next delta to compensate.
              test.delta -= timer.delta;

              if (timer.previous != etl::timer::id::NO_TIMER)
              {
                ptimers[timer.previous].next = timer.id;
              }
              break;
            }
            else
            {
              timer.delta -= test.delta;
            }

            test_id = next(test_id);
          }

          // Reached the end?
          if (test_id == etl::timer::id::NO_TIMER)
          {
            // Tag on to the tail.
            ptimers[tail].next = timer.id;
            timer.previous     = tail;
            timer.next         = etl::timer::id::NO_TIMER;
            tail               = timer.id;
          }
        }
      }

      //*******************************
      void remove(etl::timer::id::type id_, bool has_expired)
      {
        timer_data& timer = ptimers[id_];

        if (head == id_)
        {
          head = timer.next;
        }
        else
        {
          ptimers[timer.previous].next = timer.next;
        }

        if (tail == id_)
        {
          tail = timer.previous;
        }
        else
        {
          ptimers[timer.next].previous = timer.previous;
        }

        if (!has_expired)
        {
          // Adjust the next delta.
          if (timer.next != etl::timer::id::NO_TIMER)
          {
            ptimers[timer.next].delta += timer.delta;
          }
        }

        timer.previous = etl::timer::id::NO_TIMER;
        timer.next     = etl::timer::id::NO_TIMER;
        timer.delta    = etl::timer::state::Inactive;
      }

      //*******************************
      timer_data& front()
      {
        return ptimers[head];
      }

      //*******************************
      const timer_data& front() const
      {
        return ptimers[head];
      }

      //*******************************
      etl::timer::id::type begin()
      {
        return head;
      }

      //*******************************
      etl::timer::id::type next(etl::timer::id::type last)
      {
        return ptimers[last].next;
      }

      //*******************************
      void clear()
      {
        etl::timer::id::type id = begin();

        while (id != etl::timer::id::NO_TIMER)
        {
          timer_data& timer = ptimers[id];
          id                = next(id);
          timer.next        = etl::timer::id::NO_TIMER;
        }

        head = etl::timer::id::NO_TIMER;
        tail = etl::timer::id::NO_TIMER;
      }

    private:

      etl::timer::id::type head;
      etl::timer::id::type tail;

      timer_data* const ptimers;

      // Set if an attempt was ever made to insert a timer that was already
      // in the list.
      bool duplicate_insert;
    };

    // The array of timer data structures.
    timer_data* const timer_array;

    // The list of active timers.
    timer_list active_list;

    volatile bool enabled;
    uint_least8_t registered_timers;

    event_callback_type timeout_callback;

    etl::timer_data terminator;
  };

  //***************************************************************************
  /// The callback timer
  //***************************************************************************
  class intrusive_callback_timer : public etl::iintrusive_callback_timer
  {
  public:

  private:
  };
} // namespace etl

#endif
