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

#include <stdint.h>
#include <assert.h>

#include "ecl_timer.h"
#include "ecl_timer_list.h"

//*******************************************
/// Default initialisation.
//*******************************************
void ecl_timer_data_init_default(struct ecl_timer_config* ptimer_data_)
{
  assert(ptimer_data_ != 0);

  ptimer_data_->pcallback = 0;
  ptimer_data_->period    = 0;
  ptimer_data_->delta     = ECL_TIMER_INACTIVE;
  ptimer_data_->id        = ECL_TIMER_NO_TIMER;
  ptimer_data_->previous  = ECL_TIMER_NO_TIMER;
  ptimer_data_->next      = ECL_TIMER_NO_TIMER;
  ptimer_data_->repeating = ECL_TIMER_REPEATING;
}

//*******************************************
/// Parameterised initialisation.
//*******************************************
void ecl_timer_data_init(struct ecl_timer_config* ptimer_data_,
                         ecl_timer_id_t               id_,
                         void                   (*pcallback_)(),
                         ecl_timer_time_t             period_,
                         ecl_timer_mode_t             repeating_)
{
  assert(ptimer_data_ != 0);
  assert(pcallback_   != 0);

  ptimer_data_->pcallback = pcallback_;
  ptimer_data_->period    = period_;
  ptimer_data_->delta     = ECL_TIMER_INACTIVE;
  ptimer_data_->id        = id_;
  ptimer_data_->previous  = ECL_TIMER_NO_TIMER;
  ptimer_data_->next      = ECL_TIMER_NO_TIMER;
  ptimer_data_->repeating = repeating_;
}

//*******************************************
/// Returns true if the timer is active.
//*******************************************
ecl_timer_result_t ecl_timer_is_active(struct ecl_timer_config* ptimer_data_)
{
  assert(ptimer_data_ != 0);

  return (ptimer_data_->delta != ECL_TIMER_INACTIVE) ? ECL_TIMER_PASS : ECL_TIMER_FAIL;
}

//*******************************************
/// Sets the timer to the inactive state.
//*******************************************
void ecl_set_timer_inactive(struct ecl_timer_config* ptimer_data_)
{
  assert(ptimer_data_ != 0);

  ptimer_data_->delta = ECL_TIMER_INACTIVE;
}

struct ecl_time_config
{
  struct ecl_timer_config* ptimers;
  uint_least8_t max_timers;
  volatile ecl_timer_enable_t enabled;
  ECL_TIMER_TIMER_SEMAPHORE process_semaphore;
  volatile uint_least8_t registered_timers;
};

static struct ecl_time_config ecl;

void ecl_timer_init(struct ecl_timer_config* ptimers_, uint_least8_t max_timers_)
{
  assert(ptimers_ != 0);

  ecl.ptimers           = ptimers_;
  ecl.max_timers        = max_timers_;
  ecl.enabled           = 0;
  ecl.process_semaphore = 0;
  ecl.registered_timers = 0;

  int i;
  for (i = 0; i < max_timers_; ++i)
  {
    ecl_timer_data_init_default(&ecl.ptimers[i]);
  }

  ecl_timer_list_init(ecl.ptimers);
}

//*******************************************
/// Register a ptimer->
//*******************************************
ecl_timer_id_t ecl_timer_register(void             (*pcallback_)(),
                                  ecl_timer_time_t period_,
                                  ecl_timer_mode_t repeating_)
{
  assert(pcallback_ != 0);
  assert(ecl.ptimers != 0);

  ecl_timer_id_t id = ECL_TIMER_NO_TIMER;

  ECL_TIMER_DISABLE_PROCESSING(ecl.process_semaphore);

  int is_space = (ecl.registered_timers < ecl.max_timers);

  if (is_space)
  {
    // Search for the free space.
    uint_least8_t i;
    for (i = 0; i < ecl.max_timers; ++i)
    {
      struct ecl_timer_config* ptimer = &ecl.ptimers[i];

      if (ptimer->id == ECL_TIMER_NO_TIMER)
      {
        // Create in-place.
        ecl_timer_data_init(ptimer, i, pcallback_, period_, repeating_);
        ++ecl.registered_timers;
        id = i;
        break;
      }
    }
  }

  ECL_TIMER_ENABLE_PROCESSING(ecl.process_semaphore);

  return id;
}

//*******************************************
/// Unregister a ptimer->
//*******************************************
ecl_timer_result_t ecl_timer_unregister(ecl_timer_id_t id_)
{
  assert(ecl.ptimers != 0);
  
  ecl_timer_result_t result = ECL_TIMER_FAIL;

  if (id_ != ECL_TIMER_NO_TIMER)
  {
    ECL_TIMER_DISABLE_PROCESSING(ecl.process_semaphore);

    struct ecl_timer_config* ptimer = &ecl.ptimers[id_];

    if (ptimer->id != ECL_TIMER_NO_TIMER)
    {
      if (ecl_timer_is_active(ptimer))
      {
        ecl_timer_list_remove(ptimer->id, 0);

        // Reset in-place.
        ecl_timer_data_init_default(ptimer);
        --ecl.registered_timers;

        result = ECL_TIMER_PASS;
      }
    }

    ECL_TIMER_ENABLE_PROCESSING(ecl.process_semaphore);
  }

  return result;
}

//*******************************************
/// Enable/disable the ptimer->
//*******************************************
void ecl_timer_enable(ecl_timer_enable_t state_)
{
  assert(ecl.ptimers != 0);
  assert((state_ == ECL_TIMER_ENABLED) || (state_ == ECL_TIMER_DISABLED));

  ecl.enabled = state_;
}

//*******************************************
/// Get the enable/disable state.
//*******************************************
ecl_timer_result_t ecl_timer_is_running()
{
  return ecl.enabled;
}

//*******************************************
/// Clears the timer of data.
//*******************************************
void ecl_timer_clear()
{ 
  ECL_TIMER_DISABLE_PROCESSING(ecl.process_semaphore);

  ecl_timer_list_clear();

  int i;
  for (i = 0; i < ecl.max_timers; ++i)
  {
    ecl_timer_data_init_default(&ecl.ptimers[i]);
  }

  ecl.registered_timers = 0;

  ECL_TIMER_ENABLE_PROCESSING(ecl.process_semaphore);
}

//*******************************************
// Called by the timer service to indicate the
// amount of time that has elapsed since the last successful call to 'tick'.
// Returns true if the tick was processed, false if not.
//*******************************************
ecl_timer_result_t ecl_timer_tick(uint32_t count)
{
  assert(ecl.ptimers != 0);
  
  if (ecl.enabled)
  {
    if (ECL_TIMER_PROCESSING_ENABLED(ecl.process_semaphore))
    {
      // We have something to do?
      int has_active = !ecl_timer_list_empty();

      if (has_active)
      {
        while (has_active && (count >= ecl_timer_list_front()->delta))
        {
          struct ecl_timer_config* ptimer = ecl_timer_list_front();

          count -= ptimer->delta;

          ecl_timer_list_remove(ptimer->id, 1);

          if (ptimer->repeating)
          {
            // Reinsert the ptimer->
            ptimer->delta = ptimer->period;
            ecl_timer_list_insert(ptimer->id);
          }

          if (ptimer->pcallback != 0)
          {
            // Call the C callback.
            (ptimer->pcallback)();
          }

          has_active = !ecl_timer_list_empty();
        }

        if (has_active)
        {
          // Subtract any remainder from the next due timeout.
          ecl_timer_list_front()->delta -= count;
        }
      }

      return ECL_TIMER_PASS;
    }
  }

  return ECL_TIMER_FAIL;
}

//*******************************************
/// Starts a timer
//*******************************************
ecl_timer_result_t ecl_timer_start(ecl_timer_id_t id_, ecl_timer_start_t immediate_)
{
  assert(ecl.ptimers != 0);
  
  ECL_TIMER_DISABLE_PROCESSING(ecl.process_semaphore);

  ecl_timer_result_t result = ECL_TIMER_FAIL;

  // Valid timer id?
  if (id_ != ECL_TIMER_NO_TIMER)
  {
    struct ecl_timer_config* ptimer = &ecl.ptimers[id_];

    // Registered timer?
    if (ptimer->id != ECL_TIMER_NO_TIMER)
    {
      // Has a valid period.
      if (ptimer->period != ECL_TIMER_INACTIVE)
      {
        if (ecl_timer_is_active(ptimer))
        {
          ecl_timer_list_remove(ptimer->id, 0);
        }

        ptimer->delta = immediate_ ? 0 : ptimer->period;
        ecl_timer_list_insert(ptimer->id);

        result = ECL_TIMER_PASS;
      }
    }
  }

  ECL_TIMER_ENABLE_PROCESSING(ecl.process_semaphore);

  return result;
}

//*******************************************
/// Stops a timer
//*******************************************
ecl_timer_result_t ecl_timer_stop(ecl_timer_id_t id_)
{
  assert(ecl.ptimers != 0);
  
  ECL_TIMER_DISABLE_PROCESSING(ecl.process_semaphore);

  ecl_timer_result_t result = ECL_TIMER_FAIL;

  // Valid timer id?
  if (id_ != ECL_TIMER_NO_TIMER)
  {
    struct ecl_timer_config* ptimer = &ecl.ptimers[id_];

    // Registered timer?
    if (ptimer->id != ECL_TIMER_NO_TIMER)
    {
      if (ecl_timer_is_active(ptimer))
      {
        ecl_timer_list_remove(ptimer->id, 0);
        result = ECL_TIMER_PASS;
      }
    }
  }

  ECL_TIMER_ENABLE_PROCESSING(ecl.process_semaphore);

  return result;
}

//*******************************************
/// Sets a timer's period.
//*******************************************
ecl_timer_result_t ecl_timer_set_period(ecl_timer_id_t id_, ecl_timer_time_t period_)
{
  assert(ecl.ptimers != 0);
  
  if (ecl_timer_stop(id_))
  {
    ecl.ptimers[id_].period = period_;
    return ecl_timer_start(id_, 0);
  }

  return ECL_TIMER_FAIL;
}

//*******************************************
/// Sets a timer's mode.
//*******************************************
ecl_timer_result_t ecl_timer_set_mode(ecl_timer_id_t id_, ecl_timer_mode_t repeating_)
{
  assert(ecl.ptimers != 0);
  
  if (ecl_timer_stop(id_))
  {
    ecl.ptimers[id_].repeating = repeating_;
    return ecl_timer_start(id_, 0);
  }

  return ECL_TIMER_FAIL;
}




