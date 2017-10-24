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

#include "ecl_timer.h"
#include "ecl_timer_list.h"

//*************************************************************************
/// A specialised intrusive linked list for timer data.
//*************************************************************************

static ecl_timer_id_t head;
static ecl_timer_id_t tail;
static ecl_timer_id_t current;

struct ecl_timer_config* ptimers;

void ecl_timer_list_init(struct ecl_timer_config* const ptimers_)
{
  ptimers = ptimers_;
  head    = ECL_TIMER_NO_TIMER;
  tail    = ECL_TIMER_NO_TIMER;
  current = ECL_TIMER_NO_TIMER;
}

//*******************************
struct ecl_timer_config* ecl_timer_list_front()
{
  return &ptimers[head];
}

//*******************************
ecl_timer_id_t ecl_timer_list_begin()
{
  current = head;
  return current;
}

//*******************************
ecl_timer_id_t ecl_timer_list_previous(ecl_timer_id_t last)
{
  current = ptimers[last].previous;
  return current;
}

//*******************************
ecl_timer_id_t ecl_timer_list_next(ecl_timer_id_t last)
{
  current = ptimers[last].next;
  return current;
}

//*******************************
int ecl_timer_list_empty()
{
  return head == ECL_TIMER_NO_TIMER;
}

//*******************************
// Inserts the timer at the correct delta position
//*******************************
void ecl_timer_list_insert(ecl_timer_id_t id_)
{
  struct ecl_timer_config* ptimer = &ptimers[id_];

  if (head == ECL_TIMER_NO_TIMER)
  {
    // No entries yet.
    head = id_;
    tail = id_;
    ptimer->previous = ECL_TIMER_NO_TIMER;
    ptimer->next     = ECL_TIMER_NO_TIMER;
  }
  else
  {
    // We already have entries.
    ecl_timer_id_t test_id = ecl_timer_list_begin();

    while (test_id != ECL_TIMER_NO_TIMER)
    {
      struct ecl_timer_config* ptest = &ptimers[test_id];

      // Find the correct place to insert.
      if (ptimer->delta <= ptest->delta)
      {
        if (ptest->id == head)
        {
          head = ptimer->id;
        }

        // Insert before ptest->
        ptimer->previous = ptest->previous;
        ptest->previous = ptimer->id;
        ptimer->next = ptest->id;

        // Adjust the next delta to compensate.
        ptest->delta -= ptimer->delta;

        if (ptimer->previous != ECL_TIMER_NO_TIMER)
        {
          ptimers[ptimer->previous].next = ptimer->id;
        }
        break;
      }
      else
      {
        ptimer->delta -= ptest->delta;
      }

      test_id = ecl_timer_list_next(test_id);
    }

    // Reached the end?
    if (test_id == ECL_TIMER_NO_TIMER)
    {
      // Tag on to the tail.
      ptimers[tail].next = ptimer->id;
      ptimer->previous   = tail;
      ptimer->next       = ECL_TIMER_NO_TIMER;
      tail               = ptimer->id;
    }
  }
}

//*******************************
void ecl_timer_list_remove(ecl_timer_id_t id_, int has_expired)
{
  struct ecl_timer_config* ptimer = &ptimers[id_];

  if (head == id_)
  {
    head = ptimer->next;
  }
  else
  {
    ptimers[ptimer->previous].next = ptimer->next;
  }

  if (tail == id_)
  {
    tail = ptimer->previous;
  }
  else
  {
    ptimers[ptimer->next].previous = ptimer->previous;
  }

  if (!has_expired)
  {
    // Adjust the next delta.
    if (ptimer->next != ECL_TIMER_NO_TIMER)
    {
      ptimers[ptimer->next].delta += ptimer->delta;
    }
  }

  ptimer->previous = ECL_TIMER_NO_TIMER;
  ptimer->next     = ECL_TIMER_NO_TIMER;
  ptimer->delta    = ECL_TIMER_INACTIVE;
}

//*******************************
void ecl_timer_list_clear()
{
  ecl_timer_id_t id = ecl_timer_list_begin();

  while (id != ECL_TIMER_NO_TIMER)
  {
    struct ecl_timer_config* ptimer = &ptimers[id];
    id = ecl_timer_list_next(id);
    ptimer->next = ECL_TIMER_NO_TIMER;
  }

  head = ECL_TIMER_NO_TIMER;
  tail = ECL_TIMER_NO_TIMER;
  current = ECL_TIMER_NO_TIMER;
}
