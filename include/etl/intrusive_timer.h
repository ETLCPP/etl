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

#ifndef ETL_INTRUSIVE_TIMER_INCLUDED
#define ETL_INTRUSIVE_TIMER_INCLUDED

#include "platform.h"
#include "nullptr.h"
#include "timer_node.h"

#if ETL_HAS_ATOMIC
  #include "atomic.h"
#endif

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

    //*******************************************
    /// The callback type for notifying the user of the next timer that is due to expire.
    /// Either returns a pointer to the next timer that is due to expire or etl::timer_node::end() if there is no next timer.
    //*******************************************
    typedef etl::delegate<void(uint32_t delta)> time_to_next_notification_type;

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
    bool register_timer(etl::timer_node& timer_) ETL_NOEXCEPT
    {
      bool result = false;

      lock_policy.lock();

      if (!timer_.is_registered())
      {
        timer_.set_id(get_next_id());
        register_list.insert(timer_);
        timer_.set_owner(this);
        ++timer_count;
        result = true;
      }

      lock_policy.unlock();

      return result;
    }

    //*******************************************
    /// Unregister a timer.
    /// By node reference.
    //*******************************************
    bool unregister_timer(etl::timer_node& timer_) ETL_NOEXCEPT
    {
      lock_policy.lock();

      bool result = false;
      bool notify = false;

      if (timer_.is_registered_with(this))
      {
        if (timer_.is_active())
        {
          notify = (active_list.first() == &timer_);
          stop_timer(timer_);
        }

        register_list.remove(&timer_);

        --timer_count;

        result = true;
      }

      notify_about_timer_change(notify, active_list.first());

      lock_policy.unlock();

      return result;
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
      bool was_processed = false;

      if (enabled)
      {
        bool notify = false;

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
              // Calculate if we have any time left for the next timer.
              count -= timer->delta;

              // The timer has expired, so reset the delta to 0.
              timer->delta = 0;

              active_list.remove(timer);

              // If the timer is repeating then add it back to the active list.
              if (timer->get_mode() == etl::timer::mode::Repeating)
              {
                // Reinsert the timer.
                timer->delta = timer->get_interval();
                active_list.insert(timer);
              }

              expired_list.insert(timer);

              timer = active_list.first();

              has_active = !active_list.empty();

              notify = true;
            }

            if (has_active)
            {
              // Subtract any remainder from the next due timeout.
              active_list.first()->delta -= count;
            }
          }

          notify_about_timer_change(notify, active_list.first());

          was_processed = true;

          lock_policy.unlock();
        }

        // Invoke all of the expired timers.
        invoke_expired_timers();
      }

      return was_processed;
    }

    //*******************************************
    /// Starts a timer.
    //*******************************************
    bool start(etl::timer_node& timer_, bool immediate_ = false)
    {
      bool is_started = false;

      lock_policy.lock();

      if (timer_.is_registered_with(this))
      {
        // Must not be active, must have a valid interval, and must have a valid mode.
        if (!timer_.is_active() && (timer_.get_interval() != etl::timer::interval::No_Active_Interval)
            && (timer_.get_mode() != etl::timer::mode::No_Mode_Set))
        {
          active_list.insert(timer_);
          is_started = true;
        }
      }

      // Is this the next timer that is due to expire?
      bool notify = (active_list.first() == &timer_);

      lock_policy.unlock();

      if (is_started && immediate_)
      {
        // Trigger the timer immediately.
        timer_.invoke();
      }

      notify_about_timer_change(notify, &timer_);

      return is_started;
    }

    //*******************************************
    /// Stops a timer.
    //*******************************************
    bool stop(etl::timer_node& timer_) ETL_NOEXCEPT
    {
      bool is_stopped = false;

      lock_policy.lock();

      if (timer_.is_active())
      {
        stop_timer(timer_);
        is_stopped = true;
      }

      // The next timer has changed, so notify the user.
      time_to_next_notification.call_if(active_list.first()->get_delta());

      lock_policy.unlock();

      return is_stopped;
    }

    //*******************************************
    /// Sets a timer's interval.
    //*******************************************
    bool set_interval(etl::timer_node& timer_, uint32_t interval_) ETL_NOEXCEPT
    {
      bool result = false;

      lock_policy.lock();

      bool notify = false;

      if (timer_.is_registered_with(this))
      {
        if (timer_.is_active())
        {
          notify = (active_list.first() == &timer_);
          stop_timer(timer_);
        }

        timer_.set_interval(interval_);
        result = true;
      }

      notify_about_timer_change(notify, active_list.first());

      lock_policy.unlock();

      return result;
    }

    //*******************************************
    /// Sets a timer's mode.
    //*******************************************
    bool set_mode(etl::timer_node& timer_, etl::timer::mode::type mode_) ETL_NOEXCEPT
    {
      bool result = false;

      lock_policy.lock();

      bool notify = false;

      if (timer_.is_registered_with(this))
      {
        if (timer_.is_active())
        {
          notify = (active_list.first() == &timer_);
          stop_timer(timer_);
        }
        timer_.set_mode(mode_);
        result = true;
      }

      notify_about_timer_change(notify, active_list.first());

      lock_policy.unlock();

      return result;
    }

    //*******************************************
    /// Sets a timer's interval.
    //*******************************************
    bool set_config(etl::timer_node& timer_, const etl::timer_config& config_) ETL_NOEXCEPT
    {
      bool result = false;

      lock_policy.lock();

      bool notify = false;

      if (timer_.is_registered_with(this))
      {
        if (timer_.is_active())
        {
          notify = (active_list.first() == &timer_);
          stop_timer(timer_);
        }

        timer_.set_config(config_);
        result = true;
      }

      notify_about_timer_change(notify, active_list.first());

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

      if (has_active_timer())
      {
        delta = active_list.first()->get_delta();
      }

      lock_policy.unlock();

      return delta;
    }

    //*******************************************
    /// Check if a timer is registered with this timer.
    //*******************************************
    bool is_registered(const etl::timer_node& timer_) const ETL_NOEXCEPT
    {
      lock_policy.lock();

      bool is_registered = timer_.is_registered_with(this);

      lock_policy.unlock();

      return is_registered;
    }

    //*******************************************
    /// Returns the number of registered timers.
    //*******************************************
    size_t number_of_timers() const ETL_NOEXCEPT
    {
      return timer_count;
    }

    //*******************************************
    /// Sets the update notification.
    //*******************************************
    void set_next_timer_notification(time_to_next_notification_type callback) ETL_NOEXCEPT
    {
      time_to_next_notification = callback;
    }

    //*******************************************
    /// Clears the update notification.
    //*******************************************
    void clear_next_timer_notification() ETL_NOEXCEPT
    {
      time_to_next_notification.clear();
    }

    //*******************************************
    /// Finds a timer by its id.
    //*******************************************
    etl::timer_node* find_timer(etl::timer::id::type id_) ETL_NOEXCEPT
    {
      etl::timer_node* timer = register_list.first();

      while (timer != register_list.end())
      {
        if (timer->get_id() == id_)
        {
          return timer;
        }

        timer = register_list.next(timer);
      }
      return etl::timer_node::end();
    }

  private:

    //*******************************************
    /// Notifies the user of the next timer that is due to expire.
    //*******************************************
    void notify_about_timer_change(bool notify_, etl::timer_node* timer_) ETL_NOEXCEPT
    {
      if (notify_ && time_to_next_notification.is_valid())
      {
        // The next timer has changed, so notify the user.
        time_to_next_notification(timer_->get_delta());
      }
    }

    //*******************************************
    /// Gets the next available timer id.
    /// Searches the list of registered timers for the next available id.
    //*******************************************
    etl::timer::id::type get_next_id() ETL_NOEXCEPT
    {
      etl::timer::id::type next_id = 0; // IDs start at 0.

      etl::timer_node* timer = register_list.first();

      while ((timer != register_list.end()) && (timer->get_id() == next_id))
      {
        ++next_id;
        timer = register_list.next(timer);
      }

      return next_id;
    }

    //*******************************************
    /// Stops a timer without locking.
    //*******************************************
    void stop_timer(etl::timer_node& timer_) ETL_NOEXCEPT
    {
      active_list.remove(&timer_);
    }

    //*******************************************
    /// Invoke all of the expired timers.
    /// This is called after the lock has been released to avoid deadlocks.
    //*******************************************
    void invoke_expired_timers()
    {
      etl::timer_node* timer = expired_list.first();

      while (timer != expired_list.end())
      {
        timer->invoke();
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

    time_to_next_notification_type time_to_next_notification;

    bool   enabled;
    size_t timer_count;
  };
} // namespace etl

#endif
