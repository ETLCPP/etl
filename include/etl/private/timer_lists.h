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
// Definitions used by intrusive_timer.
//*****************************************************************************

namespace etl
{
  namespace private_timer
  {
    //*************************************************************************
    /// The list of timers that are registered.
    /// The list does not do any checking of parameters as it is assumed that the timer service has already done this.
    //*************************************************************************
    class register_list
    {
    public:

      //*******************************
      /// Constructor.
      //*******************************
      register_list() ETL_NOEXCEPT
        : head(end())
        , tail(end())
      {
      }

      //*******************************
      /// Returns true if the list is empty.
      //*******************************
      bool empty() const
      {
        return head == end();
      }

      //*******************************
      // Adds the timer in to the list in ID order.
      //*******************************
      void insert(etl::timer::node* new_timer) ETL_NOEXCEPT
      {
        new_timer->registered_next = end();

        if (empty())
        {
          head = new_timer;
          tail = new_timer;
        }
        else if (tail->get_id() < new_timer->get_id())
        {
          // Common case: new id is greater than every existing id, so it goes at the tail.
          tail->registered_next = new_timer;
          tail                  = new_timer;
        }
        else
        {
          etl::timer::node* current  = head;
          etl::timer::node* previous = head;

          // Find the correct place to insert the new timer in to the list.
          while ((current != end()) && (current->get_id() < new_timer->get_id()))
          {
            previous = current;
            current  = current->registered_next;
          }

          // Insert the new timer in to the list.
          new_timer->registered_next = current;

          if (current == head)
          {
            head = new_timer;
          }
          else
          {
            previous->registered_next = new_timer;
          }
        }

        // We are now the exclusive owner of this node.
        new_timer->set_owner(this);
      }

      //*******************************
      // Adds the timer in to the list.
      //*******************************
      void insert(etl::timer::node& new_timer) ETL_NOEXCEPT
      {
        insert(&new_timer);
      }

      //*******************************
      // Remove the timer from the list.
      // Returns the next timer in the list after the removed timer, or nullptr if the removed timer was the last in the list.
      //*******************************
      etl::timer::node* remove(etl::timer::node* timer_) ETL_NOEXCEPT
      {
        etl::timer::node* current  = head;
        etl::timer::node* previous = head;

        // Find the timer in the list so we can know what timer was previous to it.
        while (current != timer_)
        {
          previous = current;
          current  = current->registered_next;
        }

        // Get the next timer in the lister the timer to be removed.
        etl::timer::node* next = current->registered_next;

        // Remove it from the list.
        if (current == head)
        {
          // The timer to be removed is the head of the list.
          head = next;
        }
        else
        {
          previous->registered_next = next;
        }

        if (current == tail)
        {
          // The timer to be removed is the tail of the list.
          tail = (previous == current) ? end() : previous;
        }

        // Clear the id and next pointer of the removed timer.
        current->set_id(etl::timer::id::No_Timer);
        current->registered_next = etl::timer::node::unlinked();

        // We are no longer the owner of this node.
        current->set_owner(ETL_NULLPTR);

        return next;
      }

      //*******************************
      /// Returns the first timer in the list.
      /// If the list is empty, returns end().
      //*******************************
      etl::timer::node* first() const ETL_NOEXCEPT
      {
        return head;
      }

      //*******************************
      /// Returns the next timer in the list after the given timer.
      /// May be end() if the given timer is the last in the list.
      /// If the given timer is nullptr, returns end().
      //*******************************
      etl::timer::node* next(etl::timer::node* timer) const ETL_NOEXCEPT
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
      /// Always returns &etl::timer::node::end().
      //*******************************
      etl::timer::node* end() const ETL_NOEXCEPT
      {
        return etl::timer::node::end();
      }

      //*******************************
      /// Clear the list of entries.
      //*******************************
      void clear()
      {
        etl::timer::node* timer = head;

        while (head != end())
        {
          head = timer->registered_next;
          timer->clear();
          timer = head;
        }

        tail = end();
      }

      //*******************************************
      /// Finds a timer in a list by its address.
      /// If in the list then returns itself.
      /// If not in the list then returns end().
      //*******************************************
      etl::timer::node* find(etl::timer::node* timer_) const ETL_NOEXCEPT
      {
        etl::timer::node* node = first();

        while ((node != end()) && (node != timer_))
        {
          node = next(node);
        }

        return node;
      }

    private:

      etl::timer::node* head;
      etl::timer::node* tail;
    };

    //*************************************************************************
    /// The list of timers that are active.
    /// The list does not do any checking of parameters as it is assumed that the timer service has already done this.
    //*************************************************************************
    class active_list
    {
    public:

      //*******************************
      /// Constructor.
      //*******************************
      active_list() ETL_NOEXCEPT
        : head(end())
      {
      }

      //*******************************
      /// Returns true if the list is empty.
      //*******************************
      bool empty() const ETL_NOEXCEPT
      {
        return head == end();
      }

      //*******************************
      // Inserts the timer at the correct delta position
      //*******************************
      void insert(etl::timer::node* new_timer) ETL_NOEXCEPT
      {
        if (empty())
        {
          // No entries yet.
          head                   = new_timer;
          new_timer->active_next = end();
        }
        else if (new_timer->delta < head->delta)
        {
          // Goes before the head.
          head->delta -= new_timer->delta;
          new_timer->active_next = head;
          head                   = new_timer;
        }
        else
        {
          // We already have entries, and new_timer doesn't go before head.
          etl::timer::node* previous_timer = head;
          etl::timer::node* timer          = head->active_next;

          // Adjust the new_timer delta to take account of the head timer's delta.
          new_timer->delta -= previous_timer->delta;

          bool inserted = false;

          while (!inserted && (timer != end()))
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
            new_timer->active_next      = end();
          }
        }
      }

      //*******************************
      // Inserts the timer at the correct delta position
      //*******************************
      void insert(etl::timer::node& new_timer) ETL_NOEXCEPT
      {
        insert(&new_timer);
      }

      //*******************************
      /// Removes the selected timer from the list.
      //*******************************
      etl::timer::node* remove(etl::timer::node* timer_) ETL_NOEXCEPT
      {
        etl::timer::node* current  = head;
        etl::timer::node* previous = head;

        // Find the timer in the list so we can know what timer was previous to it.
        while (current != timer_)
        {
          previous = current;
          current  = current->active_next;
        }

        // Get the next timer in the list after the timer to be removed.
        etl::timer::node* next = current->active_next;

        // Adjust the next delta.
        if (next != end())
        {
          next->delta += current->delta;
        }

        // Remove it from the list.
        if (current == head)
        {
          // The timer to be removed is the head of the list.
          head = next;
        }
        else
        {
          previous->active_next = next;
        }

        // Clear the next pointer of the removed timer.
        timer_->active_next = etl::timer::node::unlinked();

        return next;
      }

      //*******************************
      /// Returns the first timer in the list.
      /// If the list is empty, returns end().
      //*******************************
      etl::timer::node* first() const ETL_NOEXCEPT
      {
        return head;
      }

      //*******************************
      /// Returns the next timer in the list after the given timer.
      /// May be end() if the given timer is the last in the list.
      /// If the given timer is nullptr, returns end().
      //*******************************
      etl::timer::node* next(etl::timer::node* timer) const ETL_NOEXCEPT
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
      /// Always returns &etl::timer::node::end().
      //*******************************
      etl::timer::node* end() const ETL_NOEXCEPT
      {
        return etl::timer::node::end();
      }

      //*******************************
      /// Clear the list of entries.
      //*******************************
      void clear()
      {
        etl::timer::node* timer = head;

        while (head != end())
        {
          head               = timer->active_next;
          timer->active_next = etl::timer::node::unlinked();
          timer              = head;
        }
      }

      //*******************************************
      /// Finds a timer in a list by its address.
      /// If in the list then returns itself.
      /// If not in the list then returns end().
      //*******************************************
      etl::timer::node* find(etl::timer::node* timer_) const ETL_NOEXCEPT
      {
        etl::timer::node* node = first();

        while ((node != end()) && (node != timer_))
        {
          node = next(node);
        }

        return node;
      }

    private:

      etl::timer::node* head;
    };

    //*************************************************************************
    /// The list of timers that are registered.
    /// The list does not do any checking of parameters as it is assumed that the timer service has already done this.
    //*************************************************************************
    class expired_list
    {
    public:

      //*******************************
      /// Constructor.
      //*******************************
      expired_list() ETL_NOEXCEPT
        : head(end())
        , tail(end())
      {
      }

      //*******************************
      /// Returns true if the list is empty.
      //*******************************
      bool empty() const
      {
        return head == end();
      }

      //*******************************
      // Adds the timer in to the list.
      //*******************************
      void insert(etl::timer::node* new_timer) ETL_NOEXCEPT
      {
        new_timer->expired_next = end();

        if (empty())
        {
          head = new_timer;
        }
        else
        {
          tail->expired_next = new_timer;
        }

        tail = new_timer;
      }

      //*******************************
      // Adds the timer in to the list.
      //*******************************
      void insert(etl::timer::node& new_timer) ETL_NOEXCEPT
      {
        insert(&new_timer);
      }

      //*******************************
      /// Returns the first timer in the list.
      /// If the list is empty, returns nullptr.
      //*******************************
      etl::timer::node* first() const ETL_NOEXCEPT
      {
        return head;
      }

      //*******************************
      /// Returns the next timer in the list after the given timer.
      /// May be end() if the given timer is the last in the list.
      /// If the given timer is nullptr, returns end().
      //*******************************
      etl::timer::node* next(etl::timer::node* timer) const ETL_NOEXCEPT
      {
        if (timer != end())
        {
          return timer->expired_next;
        }
        else
        {
          return end();
        }
      }

      //*******************************
      /// Returns the end of the list.
      /// Always returns &etl::timer::node::end().
      //*******************************
      etl::timer::node* end() const ETL_NOEXCEPT
      {
        return etl::timer::node::end();
      }

      //*******************************
      /// Clear the list of entries.
      //*******************************
      void clear()
      {
        etl::timer::node* timer = head;

        while (head != end())
        {
          head                = timer->expired_next;
          timer->expired_next = timer->unlinked();
          timer               = head;
        }

        tail = end();
      }

      //*******************************************
      /// Finds a timer in a list by its address.
      /// If in the list then returns itself.
      /// If not in the list then returns end().
      //*******************************************
      etl::timer::node* find(etl::timer::node* timer_) const ETL_NOEXCEPT
      {
        etl::timer::node* node = first();

        while ((node != end()) && (node != timer_))
        {
          node = next(node);
        }

        return node;
      }

    private:

      etl::timer::node* head;
      etl::timer::node* tail;
    };
  } // namespace private_timer
} // namespace etl

#endif
