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

#ifndef __ETL_C_TIMER_FRAMEWORK_LIST__
#define __ETL_C_TIMER_FRAMEWORK_LIST__

#include "ecl_timer.h"

void ecl_timer_list_init(struct ecl_timer_config* const ptimers_);
struct ecl_timer_config* ecl_timer_list_front(void);
ecl_timer_id_t ecl_timer_list_begin(void);
ecl_timer_id_t ecl_timer_list_previous(ecl_timer_id_t last);
ecl_timer_id_t ecl_timer_list_next(ecl_timer_id_t last);
int ecl_timer_list_empty(void);
void ecl_timer_list_insert(ecl_timer_id_t id_);
void ecl_timer_list_remove(ecl_timer_id_t id_, int has_expired);
void ecl_timer_list_clear(void);

#endif
