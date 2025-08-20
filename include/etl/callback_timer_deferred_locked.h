/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#ifndef ETL_CALLBACK_TIMER_DEFERRED_LOCKED_INCLUDED
#define ETL_CALLBACK_TIMER_DEFERRED_LOCKED_INCLUDED

#include "callback_timer_locked.h"
#include "etl/nullptr.h"
#include "etl/optional.h"
#include "priority_queue.h"
#include "delegate.h"

namespace etl
{
  //***************************************************************************
  /// The deferred callback timer
  //***************************************************************************
  template <uint_least8_t Max_Timers_, uint32_t Max_Handlers_>
  class callback_timer_deferred_locked : public etl::icallback_timer_locked
  {
  public:

    ETL_STATIC_ASSERT(Max_Timers_ <= 254U, "No more than 254 timers are allowed");

    typedef icallback_timer_locked::callback_type callback_type;
    typedef icallback_timer_locked::try_lock_type try_lock_type;
    typedef icallback_timer_locked::lock_type     lock_type;
    typedef icallback_timer_locked::unlock_type   unlock_type;

  private:

    class CallbackNode 
    {
    public:

      CallbackNode(callback_type &callback_,uint_least8_t priority_) : callback(callback_), priority(priority_) 
      {
      }
            
      bool operator < (const CallbackNode& p) const
      {
        return this->priority > p.priority; // comparison was inverted here to easy the code design
      }

      callback_type callback;
      uint_least8_t priority;
    };

  public:

    //*******************************************
    /// Constructor.
    //*******************************************
    callback_timer_deferred_locked()
      : icallback_timer_locked(timer_array, Max_Timers_)
    {
    }

    //*******************************************
    /// Constructor.
    //*******************************************
    callback_timer_deferred_locked(try_lock_type try_lock_, lock_type lock_, unlock_type unlock_)
      : icallback_timer_locked(timer_array, Max_Timers_)
    {
      this->set_locks(try_lock_, lock_, unlock_);
    }

    //*******************************************
    /// Handle the tick call
    //*******************************************
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
                if (!handler_queue.full())
                {
                    handler_queue.push(CallbackNode(timer.callback, timer_priorities[timer.id]));
                }
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

    //*******************************************
    /// Handles the work collected during the tick() call
    /// You can call this function after tick()
    /// or you can call this on another task to handle the timer events.
    //*******************************************
    void handle_deferred(void)
    {
      callback_type work_todo_callback;

      do
      {
        lock();
        
        if (handler_queue.empty())
        {
          work_todo_callback.clear();
        }
        else
        {
          CallbackNode &work_todo_callback_node = handler_queue.top();
          work_todo_callback = work_todo_callback_node.callback;
          handler_queue.pop();
        }

        unlock();

        work_todo_callback.call_if();
      } while (work_todo_callback.is_valid());
    }

    // Overloads

    //*******************************************
    /// Register a timer.
    //*******************************************
    etl::timer::id::type register_timer(const callback_type& callback_,
                                        uint32_t             period_,
                                        bool                 repeating_)
    {
        return register_timer(callback_, period_, repeating_, 0);
    }

    //*******************************************
    /// Register a timer with priority.
    /// priority_ 0 is highest priority, 255 is lowest
    /// Suggestion: this could be used as amonth of work to do and
    /// less work will be done in first place.
    //*******************************************
    etl::timer::id::type register_timer(const callback_type& callback_,
                                        uint32_t             period_,
                                        bool                 repeating_,
                                        uint_least8_t        priority_)
    {
        etl::timer::id::type id = icallback_timer_locked::register_timer(callback_, period_, repeating_);

        if (id != etl::timer::id::NO_TIMER)
        {
            timer_priorities[id] = priority_;
        }

        return id;
    }

  private:

    priority_queue<CallbackNode, Max_Handlers_> handler_queue;
    uint_least8_t timer_priorities[Max_Timers_];
    timer_data timer_array[Max_Timers_];
  };
}

#endif
