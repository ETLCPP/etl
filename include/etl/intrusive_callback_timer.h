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

#ifndef ETL_INTRUSIVE_TIMER_INCLUDED
#define ETL_INTRUSIVE_TIMER_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "error_handler.h"
#include "nullptr.h"
#include "static_assert.h"
#include "timer_lists.h"
#include "timer_node.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Interface for timer lock policy
  //***************************************************************************
  class timer_lock_policy
  {
  public:

    virtual void lock()      = 0;
    virtual void unlock()    = 0;
    virtual bool is_locked() = 0;
  };

  //***************************************************************************
  /// Interface for intrusive timer
  //***************************************************************************
  class intrusive_timer
  {
  public:

    typedef etl::delegate<void(etl::timer_data::id::type, uint32_t delta)> timer_update_callback_type;

    //*******************************************
    /// Constructor.
    //*******************************************
    explicit intrusive_timer(timer_lock_policy& lock_policy_)
      : lock_policy(lock_policy_)
    {
    }

    //*******************************************
    /// Register a timer.
    //*******************************************
    etl::timer::id::type register_timer(etl::timer_node& node)
    {
      return id;
    }

    //*******************************************
    /// Unregister a timer.
    //*******************************************
    bool unregister_timer(etl::timer_node& node)
    {
      bool result = false;

      if (id_ != etl::timer::id::No_Timer)
      {
        timer_data& timer = timer_array[id_];

        if (timer.id != etl::timer::id::No_Timer)
        {
          if (timer.is_active())
          {
            lock_policy.lock();
            active_list.remove(timer.id, false);
            remove_callback.call_if(timer.id);
            lock_policy.unlock();
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
      lock_policy.lock();
      active_list.clear();
      lock_policy.unlock();

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
        if (!lock_policy.is_locked())
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
      if (id_ != etl::timer::id::No_Timer)
      {
        timer_data& timer = timer_array[id_];

        // Registered timer?
        if (timer.id != etl::timer::id::No_Timer)
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
            lock_policy.unlock();

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
      if (id_ != etl::timer::id::No_Timer)
      {
        timer_data& timer = timer_array[id_];

        // Registered timer?
        if (timer.id != etl::timer::id::No_Timer)
        {
          if (timer.is_active())
          {
            ETL_DISABLE_TIMER_UPDATES;
            active_list.remove(timer.id, false);
            remove_callback.call_if(timer.id);
            lock_policy.unlock();
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
      //    if (timer.id != etl::timer::id::No_Timer)
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

  private:

    intrusive_timer(const intrusive_timer&) ETL_DELETE;
    intrusive_timer& operator=(const intrusive_timer&) ETL_DELETE;

    etl::timer_lock_policy&                  lock_policy;
    etl::private_timer::timer_register_list  register_list;
    etl::private_timer::timer_active_list    active_list;
    etl::private_timer::timer_triggered_list triggered_list;
  };
} // namespace etl

#endif
