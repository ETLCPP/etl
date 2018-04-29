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

#ifndef ETL_C_TIMER_FRAMEWORK_INCLUDED
#define ETL_C_TIMER_FRAMEWORK_INCLUDED

#include <stdint.h>

#include "ecl_user.h"

// Pass/fail results.
enum
{
  ECL_TIMER_FAIL = 0,
  ECL_TIMER_PASS = 1,
};

typedef int ecl_timer_result_t;

// Enable states.
enum
{
  ECL_TIMER_DISABLED = ECL_TIMER_FAIL,
  ECL_TIMER_ENABLED  = ECL_TIMER_PASS
};

typedef int ecl_timer_enable_t;

// Timer modes.
enum
{
  ECL_TIMER_SINGLE_SHOT = 0,
  ECL_TIMER_REPEATING   = 1
};

typedef char ecl_timer_mode_t;

// Timer start action.
enum
{
  ECL_TIMER_START_DELAYED   = 0,
  ECL_TIMER_START_IMMEDIATE = 1
};

typedef char ecl_timer_start_t;

// Timer identifiers.
enum
{
  ECL_TIMER_NO_TIMER = 255
};

typedef uint_least8_t ecl_timer_id_t;

// Timer times.
#define ECL_TIMER_INACTIVE 0xFFFFFFFF

typedef uint32_t ecl_timer_time_t;

//*************************************************************************
/// The configuration of a timer.
//*************************************************************************
struct ecl_timer_config
{
  void             (*pcallback)();
  ecl_timer_time_t  period;
  ecl_timer_time_t  delta;
  ecl_timer_id_t    id;
  uint_least8_t     previous;
  uint_least8_t     next;
  char              repeating;
};

//*************************************************************************
/// The API.
//*************************************************************************
void ecl_timer_data_init_default(struct ecl_timer_config* ptimer_data_);
void ecl_timer_data_init(struct ecl_timer_config* ptimer_data_, ecl_timer_id_t id_, void(*pcallback_)(), ecl_timer_time_t period_, ecl_timer_mode_t repeating_);
ecl_timer_result_t ecl_timer_is_active(struct ecl_timer_config* ptimer_data_);
void ecl_set_timer_inactive(struct ecl_timer_config* ptimer_data_);
void ecl_timer_init(struct ecl_timer_config* ptimers_, uint_least8_t max_timers_);
ecl_timer_id_t ecl_timer_register(void(*pcallback_)(), ecl_timer_time_t period_, ecl_timer_mode_t repeating_);
ecl_timer_result_t ecl_timer_unregister(ecl_timer_id_t id_);
void ecl_timer_enable(ecl_timer_enable_t state_);
ecl_timer_result_t ecl_timer_is_running(void);
void ecl_timer_clear(void);
ecl_timer_result_t ecl_timer_tick(uint32_t count);
ecl_timer_result_t ecl_timer_start(ecl_timer_id_t id_, ecl_timer_start_t immediate_);
ecl_timer_result_t ecl_timer_stop(ecl_timer_id_t id_);
ecl_timer_result_t ecl_timer_set_period(ecl_timer_id_t id_, ecl_timer_time_t period_);
ecl_timer_result_t ecl_timer_set_mode(ecl_timer_id_t id_, ecl_timer_mode_t repeating_);

#endif
