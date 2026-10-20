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

#ifndef ETL_TIMER_LISTS_INCLUDED
#define ETL_TIMER_LISTS_INCLUDED

#include "../platform.h"
#include "../nullptr.h"
#include "../timer.h"
#include "../timer_node.h"

#include <stdint.h>

//*****************************************************************************
// Definitions common to intrusive timers.
//*****************************************************************************

namespace etl
{
  namespace private_timer
  {
    //*************************************************************************
    /// The list of timers that are registered.
    /// The list does not do any checking of parameters as it is assumed that the timer service has already done this.
    //*************************************************************************
    class timer_register_list
    {
    public:

      //*******************************
      timer_register_list() ETL_NOEXCEPT
        : head(end())
        , tail(end())
      {
      }

      //*******************************
      bool empty() const
      {
        return head == end();
      }

      //*******************************
      // Adds the timer in to the list.
      //*******************************
      void insert(etl::timer_node* new_timer) ETL_NOEXCEPT
      {
        if (empty())
        {
          head = new_timer;
        }
        else
        {
          tail->registered_next = new_timer;
        }

        tail                       = new_timer;
        new_timer->registered_next = end();
      }

      //*******************************
      // Remove the timer from the list.
      // Returns the next timer in the list after the removed timer, or nullptr if the removed timer was the last in the list.
      //*******************************
      etl::timer_node* remove(etl::timer_node* timer) ETL_NOEXCEPT
      {
        if (timer == head)
        {
          head = timer->registered_next;

          if (head == end())
          {
            tail = end();
          }

          timer->registered_next = ETL_NULLPTR;

          return head;
        }
        else
        {
          etl::timer_node* previous = head;

          // Step through the list until we find the timer to remove, or reach the end of the list.
          while ((previous->registered_next != end()) && (previous->registered_next != timer))
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

            timer->registered_next = end();

            return previous->registered_next;
          }
          else
          {
            // Timer not found in the list.
            return end();
          }
        }
      }

      //*******************************
      /// Returns the first timer in the list.
      /// If the list is empty, returns end().
      //*******************************
      etl::timer_node* first() const ETL_NOEXCEPT
      {
        return head;
      }

      //*******************************
      /// Returns the next timer in the list after the given timer.
      /// May be end() if the given timer is the last in the list.
      /// If the given timer is nullptr, returns end().
      //*******************************
      etl::timer_node* next(etl::timer_node* timer) const ETL_NOEXCEPT
      {
        if (timer != end())
        {
          return timer->registered_next;
        }
        else
        {
          return end();
        }
      }

      //*******************************
      /// Returns the end of the list.
      /// Always returns &etl::timer_node::sentinal().
      //*******************************
      etl::timer_node* const end() const ETL_NOEXCEPT
      {
        return etl::timer_node::sentinal();
      }

      //*******************************
      /// Clear the list of entries.
      //*******************************
      void clear()
      {
        etl::timer_node* timer = head;

        while (timer != end())
        {
          timer = pop_front();
        }

        head = end();
        tail = end();
      }

    private:

      //*******************************
      // Removes the timer at the front of the list.
      // Returns the new head of the list, which may be nullptr if the list is now empty.
      //*******************************
      etl::timer_node* pop_front() ETL_NOEXCEPT
      {
        etl::timer_node* timer = head;
        head                   = timer->registered_next;
        timer->registered_next = timer->unlinked();

        return head;
      }

      etl::timer_node* head;
      etl::timer_node* tail;
    };

    //*************************************************************************
    /// The list of timers that are active.
    /// The list does not do any checking of parameters as it is assumed that the timer service has already done this.
    //*************************************************************************
    class timer_active_list
    {
    public:

      //*******************************
      timer_active_list() ETL_NOEXCEPT
        : head(end())
      {
      }

      //*******************************
      bool empty() const ETL_NOEXCEPT
      {
        return head == end();
      }

      //*******************************
      // Inserts the timer at the correct delta position
      //*******************************
      void insert(etl::timer_node* new_timer) ETL_NOEXCEPT
      {
        if (empty())
        {
          // No entries yet.
          head                   = new_timer;
          new_timer->active_next = end();
        }
        else if (new_timer->config.delta < head->config.delta)
        {
          // Goes before the head.
          head->config.delta -= new_timer->config.delta;
          new_timer->active_next = head;
          head                   = new_timer;
        }
        else
        {
          // We already have entries, and new_timer doesn't go before head.
          timer_node* previous_timer = head;
          timer_node* timer          = head->active_next;

          // Adjust the new_timer delta to take account of the head timer's delta.
          new_timer->config.delta -= previous_timer->config.delta;

          bool inserted = false;

          while (!inserted && (timer != end()))
          {
            // Find the correct place to insert.
            // The list is sorted by delta, so we need to find the first timer with a delta greater than the new timer's delta.
            if (new_timer->config.delta < timer->config.delta)
            {
              // Insert after the previous timer.
              previous_timer->active_next = new_timer;
              new_timer->active_next      = timer;

              // Adjust the next delta for the new timer's delta.
              timer->config.delta -= new_timer->config.delta;

              inserted = true;
            }
            else
            {
              // Adjust the new_timer delta to take account of the last timer's delta.
              new_timer->config.delta -= timer->config.delta;
            }

            // Move to the next timer in the list.
            previous_timer = timer;
            timer          = timer->active_next;
          }

          if (!inserted)
          {
            // Reached the end so attach to the tail.
            previous_timer->active_next = new_timer;
            new_timer->active_next      = end();
          }
        }
      }

      //*******************************
      /// Removes the selected timer from the list.
      /// No action is taken if the timer is not found.
      /// returns the next timer in the list after the removed timer, or end() if the removed timer was the last in the list.
      /// The list does not do any checking of parameters as it is assumed that the timer service has already done this.
      //*******************************
      etl::timer_node* remove(etl::timer_node* timer) ETL_NOEXCEPT
      {
        // Find the timer in the list.
        etl::timer_node* current = head;
        etl::timer_node* last    = end();

        while (current != timer && current != end())
        {
          last    = current;
          current = current->active_next;
        }

        // Not found?
        if (current == end())
        {
          return end();
        }

        etl::timer_node* next = current->active_next;

        // Adjust the next delta.
        if (next != end())
        {
          next->config.delta += current->config.delta;
        }

        // Remove it from the list.
        if (last == end())
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
      /// If the list is empty, returns end().
      //*******************************
      etl::timer_node* first() const ETL_NOEXCEPT
      {
        return head;
      }

      //*******************************
      /// Returns the next timer in the list after the given timer.
      /// May be end() if the given timer is the last in the list.
      /// If the given timer is nullptr, returns end().
      //*******************************
      etl::timer_node* next(etl::timer_node* timer) const ETL_NOEXCEPT
      {
        if (timer != end())
        {
          return timer->active_next;
        }
        else
        {
          return end();
        }
      }

      //*******************************
      /// Returns the end of the list.
      /// Always returns &etl::timer_node::sentinal().
      //*******************************
      etl::timer_node* const end() const ETL_NOEXCEPT
      {
        return etl::timer_node::sentinal();
      }

      //*******************************
      /// Clear the list of entries.
      //*******************************
      void clear()
      {
        etl::timer_node* timer = head;

        while (timer != end())
        {
          timer = pop_front();
        }

        head = end();
      }

    private:

      //*******************************
      // Removes the timer at the front of the list.
      // Returns the new head of the list, which may be nullptr if the list is now empty.
      //*******************************
      etl::timer_node* pop_front() ETL_NOEXCEPT
      {
        etl::timer_node* timer = head;
        head                   = timer->active_next;
        timer->active_next     = timer->unlinked();

        return head;
      }

      etl::timer_node* head;
    };

    //*************************************************************************
    /// The list of timers that are registered.
    //*************************************************************************
    class timer_triggered_list
    {
    public:

      //*******************************
      timer_triggered_list() ETL_NOEXCEPT
        : head(end())
        , tail(end())
      {
      }

      //*******************************
      bool empty() const
      {
        return head == end();
      }

      //*******************************
      // Adds the timer in to the list.
      //*******************************
      void insert(etl::timer_node* new_timer) ETL_NOEXCEPT
      {
        new_timer->triggered_next = end();

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
      etl::timer_node* first() const ETL_NOEXCEPT
      {
        return head;
      }

      //*******************************
      /// Returns the next timer in the list after the given timer.
      /// May be end() if the given timer is the last in the list.
      /// If the given timer is nullptr, returns end().
      //*******************************
      etl::timer_node* next(etl::timer_node* timer) const ETL_NOEXCEPT
      {
        if (timer != end())
        {
          return timer->triggered_next;
        }
        else
        {
          return end();
        }
      }

      //*******************************
      /// Returns the end of the list.
      /// Always returns &etl::timer_node::sentinal().
      //*******************************
      etl::timer_node* const end() const ETL_NOEXCEPT
      {
        return etl::timer_node::sentinal();
      }

      //*******************************
      /// Clear the list of entries.
      //*******************************
      void clear()
      {
        etl::timer_node* timer = head;

        while (timer != end())
        {
          timer = pop_front();
        }

        head = end();
        tail = end();
      }

    private:

      //*******************************
      // Removes the timer at the front of the list.
      // Returns the new head of the list, which may be nullptr if the list is now empty.
      //*******************************
      etl::timer_node* pop_front() ETL_NOEXCEPT
      {
        etl::timer_node* timer = head;
        head                   = timer->triggered_next;
        timer->triggered_next  = timer->unlinked();

        return head;
      }

      etl::timer_node* head;
      etl::timer_node* tail;
    };
  } // namespace private_timer
} // namespace etl

#endif
