///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#ifndef ETL_CHRONO_INCLUDED
#define ETL_CHRONO_INCLUDED

#define ETL_IN_CHRONO_H

#include "platform.h"

#if ETL_NOT_USING_CPP11 && !defined(ETL_IN_UNIT_TEST)
  #error NOT SUPPORTED FOR C++03 OR BELOW
#endif

#if ETL_USING_CPP11

#include "type_traits.h"
#include "integral_limits.h"
#include "hash.h"

#include <stdint.h>
#include <time.h>

namespace etl
{
  namespace chrono
  {
    template <typename TRep>
    struct treat_as_floating_point : etl::is_floating_point<TRep>
    {
    };

#if ETL_USING_CPP17
    template <typename TRep>
    constexpr bool treat_as_floating_point_v = treat_as_floating_point<TRep>::value;
#endif
  }

  // Use the same type as defined in time.h.
  using time_t = ::time_t;
}

#include "private/chrono/last_spec.h"
#include "private/chrono/duration.h"
#include "private/chrono/time_point.h"
#include "private/chrono/clocks.h"
#include "private/chrono/day.h"
#include "private/chrono/weekday.h"
#include "private/chrono/month.h"
#include "private/chrono/month_day.h"
#include "private/chrono/month_weekday.h"
#include "private/chrono/year.h"
#include "private/chrono/year_month.h"
#include "private/chrono/year_month_day.h"
#include "private/chrono/year_month_weekday.h"
#include "private/chrono/hh_mm_ss.h"
#include "private/chrono/operators.h"
#include "private/chrono/time_zone.h"

namespace etl
{
  namespace chrono 
  {
    using namespace literals::chrono_literals;
  }
}

#endif

#undef ETL_IN_CHRONO_H

#endif
