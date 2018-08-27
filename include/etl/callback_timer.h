/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 jwellbelove

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

#include <stdint.h>
#include <new>

#include "platform.h"
#include "algorithm.h"
#include "nullptr.h"
#include "function.h"
#include "static_assert.h"
#include "timer.h"
#include "atomic.h"

#undef ETL_FILE
#define ETL_FILE "43"

#if !defined(ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK) && !defined(ETL_CALLBACK_TIMER_USE_INTERRUPT_LOCK)
  #error ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK or ETL_CALLBACK_TIMER_USE_INTERRUPT_LOCK not defined
#endif

#if defined(ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK) && defined(ETL_CALLBACK_TIMER_USE_INTERRUPT_LOCK)
  #error Only define one of ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK or ETL_CALLBACK_TIMER_USE_INTERRUPT_LOCK
#endif

#if defined(ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK)
  #define ETL_DISABLE_TIMER_UPDATES (++process_semaphore)
  #define ETL_ENABLE_TIMER_UPDATES  (--process_semaphore)
  #define ETL_TIMER_UPDATES_ENABLED (process_semaphore.load() == 0)
#endif

#if defined(ETL_CALLBACK_TIMER_USE_INTERRUPT_LOCK)
  #if !defined(ETL_CALLBACK_TIMER_DISABLE_INTERRUPTS) || !defined(ETL_CALLBACK_TIMER_ENABLE_INTERRUPTS)
    #error ETL_CALLBACK_TIMER_DISABLE_INTERRUPTS and/or ETL_CALLBACK_TIMER_ENABLE_INTERRUPTS not defined
  #endif

  #define ETL_DISABLE_TIMER_UPDATES (ETL_CALLBACK_TIMER_DISABLE_INTERRUPTS)
  #define ETL_ENABLE_TIMER_UPDATES  (ETL_CALLBACK_TIMER_ENABLE_INTERRUPTS)
  #define ETL_TIMER_UPDATES_ENABLED true
#endif

namespace etl
{
  //*************************************************************************
  /// The configuration of a timer.
  struct callback_timer_data
  {
    //*******************************************
    callback_timer_data()
      : p_callback(nullptr),
        period(0),
        delta(etl::timer::state::INACTIVE),
        id(etl::timer::id::NO_TIMER),
        previous(etl::timer::id::NO_TIMER),
        next(etl::timer::id::NO_TIMER),
        repeating(true),
        has_c_callback(true)
    {
    }

    //*******************************************
    /// C function callback
    //*******************************************
    callback_timer_data(etl::timer::id::type id_,
                        void                 (*p_callback_)(),
                        uint32_t             period_,
                        bool                 repeating_)
      : p_callback(reinterpret_cast<void*>(p_callback_)),
        period(period_),
        delta(etl::timer::state::INACTIVE),
        id(id_),
        previous(etl::timer::id::NO_TIMER),
        next(etl::timer::id::NO_TIMER),
        repeating(repeating_),
        has_c_callback(true)
    {
    }

    //*******************************************
    /// ETL function callback
    //*******************************************
    callback_timer_data(etl::timer::id::type  id_,
                        etl::ifunction<void>& callback_,
                        uint32_t              period_,
                        bool                  repeating_)
      : p_callback(reinterpret_cast<void*>(&callback_)),
        period(period_),
        delta(etl::timer::state::INACTIVE),
        id(id_),
        previous(etl::timer::id::NO_TIMER),
        next(etl::timer::id::NO_TIMER),
        repeating(repeating_),
        has_c_callback(false)
    {
    }

    //*******************************************
    /// Returns true if the timer is active.
    //*******************************************
    bool is_active() const
    {
      return delta != etl::timer::state::INACTIVE;
    }

    //*******************************************
    /// Sets the timer to the inactive state.
    //*******************************************
    void set_inactive()
    {
      delta = etl::timer::state::INACTIVE;
    }

    void*                 p_callback;
    uint32_t              period;
    uint32_t              delta;
    etl::timer::id::type  id;
    uint_least8_t         previous;
    uint_least8_t         next;
    bool                  repeating;
    bool                  has_c_callback;

  private:

    // Disabled.
    callback_timer_data(const callback_timer_data& other);
    callback_timer_data& operator =(const callback_timer_data& other);
  };

  namespace private_callback_timer
  {
    //*************************************************************************
    /// A specialised intrusive linked list for timer data.
    //*************************************************************************
    class list
    {
    public:

      //*******************************
      list(etl::callback_timer_data* ptimers_)
        : head(etl::timer::id::NO_TIMER),
          tail(etl::timer::id::NO_TIMER),
          current(etl::timer::id::NO_TIMER),
          ptimers(ptimers_)
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
        etl::callback_timer_data& timer = ptimers[id_];

        if (head == etl::timer::id::NO_TIMER)
        {
          // No entries yet.
          head = id_;
          tail = id_;
          timer.previous = etl::timer::id::NO_TIMER;
          timer.next     = etl::timer::id::NO_TIMER;
        }
        else
        {
          // We already have entries.
          etl::timer::id::type test_id = begin();

          while (test_id != etl::timer::id::NO_TIMER)
          {
            etl::callback_timer_data& test = ptimers[test_id];

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
        etl::callback_timer_data& timer = ptimers[id_];

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
        timer.delta    = etl::timer::state::INACTIVE;
      }

      //*******************************
      etl::callback_timer_data& front()
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
          etl::callback_timer_data& timer = ptimers[id];
          id = next(id);
          timer.next = etl::timer::id::NO_TIMER;
        }

        head    = etl::timer::id::NO_TIMER;
        tail    = etl::timer::id::NO_TIMER;
        current = etl::timer::id::NO_TIMER;
      }

    private:

      etl::timer::id::type head;
      etl::timer::id::type tail;
      etl::timer::id::type current;

      etl::callback_timer_data* const ptimers;
    };
  }

  //***************************************************************************
  /// Interface for callback timer
  //***************************************************************************
  class icallback_timer
  {
  public:

    //*******************************************
    /// Register a timer.
    //*******************************************
    etl::timer::id::type register_timer(void     (*p_callback_)(),
                                        uint32_t period_,
                                        bool     repeating_)
    {
      etl::timer::id::type id = etl::timer::id::NO_TIMER;

      bool is_space = (registered_timers < MAX_TIMERS);

      if (is_space)
      {
        // Search for the free space.
        for (uint_least8_t i = 0; i < MAX_TIMERS; ++i)
        {
          etl::callback_timer_data& timer = timer_array[i];

          if (timer.id == etl::timer::id::NO_TIMER)
          {
            // Create in-place.
            new (&timer) callback_timer_data(i, p_callback_, period_, repeating_);
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
    etl::timer::id::type register_timer(etl::ifunction<void>& callback_,
                                        uint32_t              period_,
                                        bool                  repeating_)
    {
      etl::timer::id::type id = etl::timer::id::NO_TIMER;

      bool is_space = (registered_timers < MAX_TIMERS);

      if (is_space)
      {
        // Search for the free space.
        for (uint_least8_t i = 0; i < MAX_TIMERS; ++i)
        {
          etl::callback_timer_data& timer = timer_array[i];

          if (timer.id == etl::timer::id::NO_TIMER)
          {
            // Create in-place.
            new (&timer) callback_timer_data(i, callback_, period_, repeating_);
            ++registered_timers;
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
        etl::callback_timer_data& timer = timer_array[id_];

        if (timer.id != etl::timer::id::NO_TIMER)
        {
          if (timer.is_active())
          {
            ETL_DISABLE_TIMER_UPDATES;
            active_list.remove(timer.id, false);
            ETL_ENABLE_TIMER_UPDATES;
          }

          // Reset in-place.
          new (&timer) callback_timer_data();
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
        new (&timer_array[i]) callback_timer_data();
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
              etl::callback_timer_data& timer = active_list.front();

              count -= timer.delta;

              active_list.remove(timer.id, true);

              if (timer.repeating)
              {
                // Reinsert the timer.
                timer.delta = timer.period;
                active_list.insert(timer.id);
              }

              if (timer.p_callback != nullptr)
              {
                if (timer.has_c_callback)
                {
                  // Call the C callback.
                  reinterpret_cast<void(*)()>(timer.p_callback)();
                }
                else
                {
                  // Call the function wrapper callback.
                  (*reinterpret_cast<etl::ifunction<void>*>(timer.p_callback))();
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
        etl::callback_timer_data& timer = timer_array[id_];

        // Registered timer?
        if (timer.id != etl::timer::id::NO_TIMER)
        {         
          // Has a valid period.
          if (timer.period != etl::timer::state::INACTIVE)
          {
            ETL_DISABLE_TIMER_UPDATES;
            if (timer.is_active())
            {
              active_list.remove(timer.id, false);
            }

            timer.delta = immediate_ ? 0 : timer.period;
            active_list.insert(timer.id);
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
        etl::callback_timer_data& timer = timer_array[id_];

        // Registered timer?
        if (timer.id != etl::timer::id::NO_TIMER)
        {
          if (timer.is_active())
          {
            ETL_DISABLE_TIMER_UPDATES;
            active_list.remove(timer.id, false);
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

  protected:

    //*******************************************
    /// Constructor.
    //*******************************************
    icallback_timer(callback_timer_data* const timer_array_, const uint_least8_t  MAX_TIMERS_)
      : timer_array(timer_array_),
        active_list(timer_array_),
        enabled(false),
#if defined(ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK)
        process_semaphore(0),
#endif
        registered_timers(0),
        MAX_TIMERS(MAX_TIMERS_)
    {
    }

  private:

    // The array of timer data structures.
    callback_timer_data* const timer_array;

    // The list of active timers.
    private_callback_timer::list active_list;

    volatile bool enabled;
#if defined(ETL_CALLBACK_TIMER_USE_ATOMIC_LOCK)
    volatile etl::timer_semaphore_t process_semaphore;
#endif
    volatile uint_least8_t registered_timers;

  public:

    const uint_least8_t MAX_TIMERS;
  };

  //***************************************************************************
  /// The callback timer
  //***************************************************************************
  template <const uint_least8_t MAX_TIMERS_>
  class callback_timer : public etl::icallback_timer
  {
  public:

    ETL_STATIC_ASSERT(MAX_TIMERS_ <= 254, "No more than 254 timers are allowed");

    //*******************************************
    /// Constructor.
    //*******************************************
    callback_timer()
      : icallback_timer(timer_array, MAX_TIMERS_)
    {
    }

  private:

    callback_timer_data timer_array[MAX_TIMERS_];
  };
}

#undef ETL_DISABLE_TIMER_UPDATES
#undef ETL_ENABLE_TIMER_UPDATES
#undef ETL_TIMER_UPDATES_ENABLED

#undef ETL_FILE

#endif
