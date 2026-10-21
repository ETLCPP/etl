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
#include "delegate.h"
#include "error_handler.h"
#include "nullptr.h"
#include "static_assert.h"
#include "timer_node.h"

#include "private/timer_lists.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Interface for timer lock policy
  //***************************************************************************
  class timer_lock_policy
  {
  public:

    virtual void lock() ETL_NOEXCEPT      = 0;
    virtual void unlock() ETL_NOEXCEPT    = 0;
    virtual bool is_locked() ETL_NOEXCEPT = 0;
  };

  //***************************************************************************
  /// Interface for intrusive timer
  //***************************************************************************
  class intrusive_timer
  {
  public:

    typedef etl::delegate<void(etl::timer::id::type, uint32_t delta)> timer_update_callback_type;

    //*******************************************
    /// Constructor.
    //*******************************************
    explicit intrusive_timer(timer_lock_policy& lock_policy_) ETL_NOEXCEPT
      : lock_policy(lock_policy_)
      , register_list()
      , active_list()
      , expired_list()
      , enabled(false)
      , timer_count(0U)
    {
    }

    //*******************************************
    /// Register a timer.
    /// It will not be started until the start() method is called.
    //*******************************************
    bool register_timer(etl::timer_node& node) ETL_NOEXCEPT
    {
      etl::timer::id::type id     = node.get_id();
      bool                 result = false;

      if (id != etl::timer::id::No_Timer)
      {
        lock_policy.lock();

        // Remove any existing timer with the same id.
        active_list.remove(id);
        register_list.remove(id);

        // Register the new timer.
        register_list.insert(&node);

        lock_policy.unlock();

        result = true;
        ++timer_count;
      }

      return result;
    }

    //*******************************************
    /// Unregister a timer.
    /// By id.
    //*******************************************
    bool unregister_timer(etl::timer::id::type id) ETL_NOEXCEPT
    {
      bool result = false;

      if (id != etl::timer::id::No_Timer)
      {
        lock_policy.lock();

        active_list.remove(id);
        register_list.remove(id);

        lock_policy.unlock();

        result = true;
        --timer_count;
      }

      return result;
    }

    //*******************************************
    /// Unregister a timer.
    /// By node reference.
    //*******************************************
    bool unregister_timer(etl::timer_node& node) ETL_NOEXCEPT
    {
      etl::timer::id::type id = node.get_id();

      return unregister_timer(id);
    }

    //*******************************************
    /// Enable/disable the timer.
    //*******************************************
    void enable(bool state_) ETL_NOEXCEPT
    {
      enabled = state_;
    }

    //*******************************************
    /// Get the enable/disable state.
    //*******************************************
    bool is_running() const ETL_NOEXCEPT
    {
      return enabled;
    }

    //*******************************************
    /// Clears the timer of data.
    //*******************************************
    void clear() ETL_NOEXCEPT
    {
      lock_policy.lock();

      active_list.clear();
      register_list.clear();

      lock_policy.unlock();
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
          lock_policy.lock();

          // We have something to do?
          bool has_active = !active_list.empty();

          if (has_active)
          {
            // Process timers that have expired.
            etl::timer_node* timer = active_list.first();

            while (has_active && (count >= timer->delta))
            {
              count -= timer->delta;

              active_list.remove(timer->get_id());

              // If the timer is repeating then add it back to the active list.
              if (timer->get_mode() == etl::timer::mode::Repeating)
              {
                // Reinsert the timer.
                timer->delta = timer->get_period();
                active_list.insert(timer);
              }

              expired_list.insert(timer);

              timer = active_list.first();

              has_active = !active_list.empty();
            }

            if (has_active)
            {
              // Subtract any remainder from the next due timeout.
              active_list.first()->delta -= count;
            }

            lock_policy.unlock();

            // Invoke all of the expired timers.
            invoke_expired_timers();
          }

          return true;
        }
      }

      return false;
    }

    //*******************************************
    /// Starts a timer.
    //*******************************************
    bool start(etl::timer::id::type id_, bool immediate_ = false) ETL_NOEXCEPT
    {
      bool is_started = false;

      lock_policy.lock();

      etl::timer_node* timer = find_timer(register_list, id_);

      if (timer != register_list.end())
      {
        active_list.insert(timer);
        is_started = true;
      }

      lock_policy.unlock();

      if (is_started && immediate_)
      {
        // Trigger the timer immediately.
        timer->invoke(timer->get_config());
      }

      return is_started;
    }

    //*******************************************
    /// Stops a timer.
    //*******************************************
    bool stop(etl::timer::id::type id_) ETL_NOEXCEPT
    {
      bool is_stopped = false;

      lock_policy.lock();

      is_stopped = (stop_impl(id_) != active_list.end());

      lock_policy.unlock();

      return is_stopped;
    }

    //*******************************************
    /// Sets a timer's period.
    //*******************************************
    bool set_period(etl::timer::id::type id_, uint32_t period_) ETL_NOEXCEPT
    {
      bool result = false;

      lock_policy.lock();

      stop_impl(id_);

      etl::timer_node* timer = find_timer(register_list, id_);

      if (timer != active_list.end())
      {
        timer->set_period(period_);
        result = true;
      }

      lock_policy.unlock();

      return result;
    }

    //*******************************************
    /// Sets a timer's mode.
    //*******************************************
    bool set_mode(etl::timer::id::type id_, bool mode_) ETL_NOEXCEPT
    {
      bool result = false;

      lock_policy.lock();

      stop_impl(id_);

      etl::timer_node* timer = find_timer(register_list, id_);

      if (timer != active_list.end())
      {
        timer->set_mode(mode_);
        result = true;
      }

      lock_policy.unlock();

      return result;
    }

    //*******************************************
    /// Check if there is an active timer.
    //*******************************************
    bool has_active_timer() const ETL_NOEXCEPT
    {
      lock_policy.lock();
      bool has_active = !active_list.empty();
      lock_policy.unlock();

      return has_active;
    }

    //*******************************************
    /// Get the time to the next timer event.
    /// Returns etl::timer::interval::No_Active_Interval if there is no active
    /// timer.
    //*******************************************
    uint32_t time_to_next() const ETL_NOEXCEPT
    {
      uint32_t delta = static_cast<uint32_t>(etl::timer::interval::No_Active_Interval);

      lock_policy.lock();

      if (!active_list.empty())
      {
        delta = active_list.first()->get_delta();
      }

      lock_policy.unlock();

      return delta;
    }

    //*******************************************
    /// Checks if a timer is currently active.
    /// Returns <b>true</b> if the timer is active, otherwise <b>false</b>.
    //*******************************************
    bool is_active(etl::timer::id::type id_) const ETL_NOEXCEPT
    {
      if (id_ != etl::timer::id::No_Timer)
      {
        lock_policy.lock();
        bool is_active = (find_timer(active_list, id_) != active_list.end());
        lock_policy.unlock();

        return is_active;
      }

      return false;
    }

    //*******************************************
    /// Returns the number of registered timers.
    //*******************************************
    size_t number_of_registered_timers() const ETL_NOEXCEPT
    {
      return timer_count;
    }

  private:

    //*******************************************
    /// Stops a timer without locking.
    /// Returns the timer node if it was found, otherwise returns end().
    //*******************************************
    etl::timer_node* stop_impl(etl::timer::id::type id_) ETL_NOEXCEPT
    {
      return active_list.remove(id_);
    }

    //*******************************************
    /// Finds a timer in a list by its id.
    //*******************************************
    template <typename TList>
    etl::timer_node* find_timer(TList& timer_list, etl::timer::id::type id_) const ETL_NOEXCEPT
    {
      etl::timer_node* result = timer_list.end();

      if (id_ != etl::timer::id::No_Timer)
      {
        timer_node* timer = timer_list.first();

        while ((timer != timer_list.end()) && (timer->get_config().get_id() != id_))
        {
          timer = timer_list.next(timer);
        }

        result = timer;
      }

      return result;
    }

    //*******************************************
    /// Invokes the callbacks for all expired timers.
    //*******************************************
    void invoke_expired_timers()
    {
      etl::timer_node* timer = expired_list.first();

      while (timer != expired_list.end())
      {
        const etl::timer_config& config = timer->get_config();

        timer->invoke(config);

        timer = expired_list.next(timer);
      }

      expired_list.clear();
    }

    intrusive_timer(const intrusive_timer&) ETL_DELETE;
    intrusive_timer& operator=(const intrusive_timer&) ETL_DELETE;

    etl::timer_lock_policy&                 lock_policy;
    etl::private_timer::timer_register_list register_list;
    etl::private_timer::timer_active_list   active_list;
    etl::private_timer::timer_expired_list  expired_list;

    bool   enabled;
    size_t timer_count;
  };
} // namespace etl

#endif
