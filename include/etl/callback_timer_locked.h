/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 John Wellbelove

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

#ifndef ETL_CALLBACK_TIMER_LOCKED_INCLUDED
#define ETL_CALLBACK_TIMER_LOCKED_INCLUDED

#include "platform.h"
#include "algorithm.h"
#include "nullptr.h"
#include "delegate.h"
#include "static_assert.h"
#include "timer.h"
#include "error_handler.h"
#include "placement_new.h"

#include <stdint.h>

namespace etl
{
  //***************************************************************************
  /// Interface for callback timer
  //***************************************************************************
  class icallback_timer_locked
  {
  public:

    typedef etl::delegate<void(void)> callback_type;
    typedef etl::delegate<bool(void)> try_lock_type;
    typedef etl::delegate<void(void)> lock_type;
    typedef etl::delegate<void(void)> unlock_type;

    //*******************************************
    /// Register a timer.
    //*******************************************
    etl::timer::id::type register_timer(const callback_type& callback_,
                                        uint32_t             period_,
                                        bool                 repeating_)
    {
      etl::timer::id::type id = etl::timer::id::NO_TIMER;

      bool is_space = (number_of_registered_timers < MAX_TIMERS);

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
            ++number_of_registered_timers;
            id = i;
            break;
          }
        }
      }

      return id;
    }

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
            lock();
            active_list.remove(timer.id, false);
            unlock();
          }

          // Reset in-place.
          new (&timer) timer_data();
          --number_of_registered_timers;

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
      lock();
      active_list.clear();
      unlock();

      for (uint8_t i = 0U; i < MAX_TIMERS; ++i)
      {
        ::new (&timer_array[i]) timer_data();
      }

      number_of_registered_timers = 0;
    }

    //*******************************************
    // Called by the timer service to indicate the
    // amount of time that has elapsed since the last successful call to 'tick'.
    // Returns true if the tick was processed,
    // false if not.
    //*******************************************
    virtual bool tick(uint32_t count) = 0;

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
            lock();
            if (timer.is_active())
            {
              active_list.remove(timer.id, false);
            }

            timer.delta = immediate_ ? 0U : timer.period;
            active_list.insert(timer.id);
            unlock();

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
            lock();
            active_list.remove(timer.id, false);
            unlock();
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
    /// Sets the lock and unlock delegates.
    //*******************************************
    void set_locks(try_lock_type try_lock_, lock_type lock_, lock_type unlock_)
    {
      try_lock = try_lock_;
      lock     = lock_;
      unlock   = unlock_;
    }

    //*******************************************
    /// Check if there is an active timer.
    //*******************************************
    bool has_active_timer() const
    {
      lock();
      bool result = !active_list.empty();
      unlock();

      return result;
    }

    //*******************************************
    /// Get the time to the next timer event.
    /// Returns etl::timer::interval::No_Active_Interval if there is no active timer.
    //*******************************************
    uint32_t time_to_next() const
    {
      uint32_t delta = static_cast<uint32_t>(etl::timer::interval::No_Active_Interval);

      lock();
      if (!active_list.empty())
      {
        delta = active_list.front().delta;
      }
      unlock();

      return delta;
    }

    //*******************************************
    /// Checks if a timer is currently active.
    /// Returns <b>true</b> if the timer is active, otherwise <b>false</b>.
    //*******************************************
    bool is_active(etl::timer::id::type id_) const
    {
      bool result = false;

      // Valid timer id?
      if (is_valid_timer_id(id_))
      {
        if (has_active_timer())
        {
          lock();
          const timer_data& timer = timer_array[id_];

          // Registered timer?
          if (timer.id != etl::timer::id::NO_TIMER)
          {
            result = timer.is_active();
          }
          unlock();
        }
      }

      return result;
    }

  protected:

    //*************************************************************************
    /// The configuration of a timer.
    struct timer_data
    {
      //*******************************************
      timer_data()
        : callback()
        , period(0U)
        , delta(etl::timer::state::Inactive)
        , id(etl::timer::id::NO_TIMER)
        , previous(etl::timer::id::NO_TIMER)
        , next(etl::timer::id::NO_TIMER)
        , repeating(true)
      {
      }

      //*******************************************
      /// ETL delegate callback
      //*******************************************
      timer_data(etl::timer::id::type id_,
                 callback_type        callback_,
                 uint32_t             period_,
                 bool                 repeating_)
        : callback(callback_)
        , period(period_)
        , delta(etl::timer::state::Inactive)
        , id(id_)
        , previous(etl::timer::id::NO_TIMER)
        , next(etl::timer::id::NO_TIMER)
        , repeating(repeating_)
      {
      }

      //*******************************************
      /// Returns true if the timer is active.
      //*******************************************
      bool is_active() const
      {
        return delta != etl::timer::state::Inactive;
      }

      //*******************************************
      /// Sets the timer to the inactive state.
      //*******************************************
      void set_inactive()
      {
        delta = etl::timer::state::Inactive;
      }

      callback_type        callback;
      uint32_t             period;
      uint32_t             delta;
      etl::timer::id::type id;
      uint_least8_t        previous;
      uint_least8_t        next;
      bool                 repeating;

    private:

      // Disabled.
      timer_data(const timer_data& other) ETL_DELETE;
      timer_data& operator =(const timer_data& other) ETL_DELETE;
    };

    //*******************************************
    /// Constructor.
    //*******************************************
    icallback_timer_locked(timer_data* const timer_array_, const uint_least8_t  MAX_TIMERS_)
      : timer_array(timer_array_),
        active_list(timer_array_),
        enabled(false),
        number_of_registered_timers(0U),
        MAX_TIMERS(MAX_TIMERS_)
    {
    }

  private:

    //*************************************************************************
    /// A specialised intrusive linked list for timer data.
    //*************************************************************************
    class timer_list
    {
    public:

      //*******************************
      timer_list(timer_data* ptimers_)
        : head(etl::timer::id::NO_TIMER)
        , tail(etl::timer::id::NO_TIMER)
        , current(etl::timer::id::NO_TIMER)
        , ptimers(ptimers_)
      {
      }

      //*******************************
      bool empty() const
      {
        return head == etl::timer::id::NO_TIMER;
      }

      //*******************************
      // Inserts the timer at the correct delta position
      //*******************************
      void insert(etl::timer::id::type id_)
      {
        timer_data& timer = ptimers[id_];

        if (head == etl::timer::id::NO_TIMER)
        {
          // No entries yet.
          head = id_;
          tail = id_;
          timer.previous = etl::timer::id::NO_TIMER;
          timer.next = etl::timer::id::NO_TIMER;
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
              test.previous = timer.id;
              timer.next = test.id;

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
            timer.previous = tail;
            timer.next = etl::timer::id::NO_TIMER;
            tail = timer.id;
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
        timer.next = etl::timer::id::NO_TIMER;
        timer.delta = etl::timer::state::Inactive;
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
        current = head;
        return current;
      }

      //*******************************
      etl::timer::id::type previous(etl::timer::id::type last)
      {
        current = ptimers[last].previous;
        return current;
      }

      //*******************************
      etl::timer::id::type next(etl::timer::id::type last)
      {
        current = ptimers[last].next;
        return current;
      }

      //*******************************
      void clear()
      {
        etl::timer::id::type id = begin();

        while (id != etl::timer::id::NO_TIMER)
        {
          timer_data& timer = ptimers[id];
          id = next(id);
          timer.next = etl::timer::id::NO_TIMER;
        }

        head = etl::timer::id::NO_TIMER;
        tail = etl::timer::id::NO_TIMER;
        current = etl::timer::id::NO_TIMER;
      }

    private:

      etl::timer::id::type head;
      etl::timer::id::type tail;
      etl::timer::id::type current;

      timer_data* const ptimers;
    };

    //*******************************************
    /// Check that the timer id is valid.
    //*******************************************
    bool is_valid_timer_id(etl::timer::id::type id_) const
    {
      return (id_ < MAX_TIMERS);
    }

    // The array of timer data structures.
    timer_data* const timer_array;

    // The list of active timers.
    timer_list active_list;

    bool enabled;
    uint_least8_t number_of_registered_timers;

    try_lock_type try_lock; ///< The callback that tries to lock.
    lock_type     lock;     ///< The callback that locks.
    unlock_type   unlock;   ///< The callback that unlocks.

  public:
    template <uint_least8_t>
    friend class callback_timer_locked;

    template <uint_least8_t, uint32_t>
    friend class callback_timer_deferred_locked;

    const uint_least8_t MAX_TIMERS;
  };

  //***************************************************************************
  /// The callback timer
  //***************************************************************************
  template <uint_least8_t MAX_TIMERS_>
  class callback_timer_locked : public etl::icallback_timer_locked
  {
  public:

    ETL_STATIC_ASSERT(MAX_TIMERS_ <= 254U, "No more than 254 timers are allowed");

    typedef icallback_timer_locked::callback_type callback_type;
    typedef icallback_timer_locked::try_lock_type try_lock_type;
    typedef icallback_timer_locked::lock_type     lock_type;
    typedef icallback_timer_locked::unlock_type   unlock_type;

    //*******************************************
    /// Constructor.
    //*******************************************
    callback_timer_locked()
      : icallback_timer_locked(timer_array, MAX_TIMERS_)
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    callback_timer_locked(try_lock_type try_lock_, lock_type lock_, unlock_type unlock_)
      : icallback_timer_locked(timer_array, MAX_TIMERS_)
    {
      this->set_locks(try_lock_, lock_, unlock_);
    }

    // Implement virtual functions

    bool tick(uint32_t count) final
    {
      if (enabled)
      {
        if (try_lock())
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

              if (timer.callback.is_valid())
              {
                timer.callback();
              }

              if (timer.repeating)
              {
                // Reinsert the timer.
                timer.delta = timer.period;
                active_list.insert(timer.id);
              }

              has_active = !active_list.empty();
            }

            if (has_active)
            {
              // Subtract any remainder from the next due timeout.
              active_list.front().delta -= count;
            }
          }

          unlock();

          return true;
        }
      }

      return false;
    }

  private:

    timer_data timer_array[MAX_TIMERS_];
  };
}

#endif
