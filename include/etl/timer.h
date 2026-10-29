/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2017 John Wellbelove

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

#ifndef ETL_TIMER_INCLUDED
#define ETL_TIMER_INCLUDED

#include "platform.h"
#include "atomic.h"
#include "limits.h"

#include <stdint.h>

//*****************************************************************************
// Definitions common to timers.
//*****************************************************************************

namespace etl
{
#ifdef ETL_TIMER_SEMAPHORE_TYPE
  typedef ETL_TIMER_SEMAPHORE_TYPE timer_semaphore
#else
  #if ETL_HAS_ATOMIC
  typedef etl::atomic_uint32_t timer_semaphore_t;
  #endif
#endif

    //***************************************************************************
    /// Common definitions for the timer framework.
    //***************************************************************************
    namespace timer
  {
    // Timer modes.
    struct mode
    {
      enum
      {
        SINGLE_SHOT = 0, // Deprecated
        REPEATING   = 1, // Deprecated

        Single_Shot = 0,
        Repeating   = 1,
        No_Mode     = 2
      };

      typedef uint_least8_t type;
    };

    // Timer start status.
    struct start
    {
      enum
      {
        DELAYED   = false, // Deprecated
        IMMEDIATE = true,

        Delayed   = false, // Deprecated
        Immediate = true
      };

      typedef bool type;
    };

    // Timer id.
    struct id
    {
      typedef uint_least8_t type;

      enum
      {
        NO_TIMER = etl::integral_limits<type>::max, // Deprecated
        No_Timer = etl::integral_limits<type>::max
      };
    };

    // Timer state.
    struct state
    {
      typedef uint32_t type;

      enum
      {
        INACTIVE = etl::integral_limits<type>::max, // Deprecated
        Inactive = etl::integral_limits<type>::max
      };
    };

    // Timer time interval.
    struct interval
    {
      typedef uint32_t type;

      enum
      {
        No_Active_Interval = etl::integral_limits<type>::max, // Deprecated
        No_Interval        = etl::integral_limits<type>::max
      };
    };

    // Timer owner.
    struct owner
    {
      typedef void* type;

#if ETL_USING_CPP11
      static ETL_CONSTANT type No_Owner = ETL_NULLPTR;
#else
      static ETL_CONSTANT type No_Owner;
#endif
    };

#if ETL_NOT_USING_CPP11
    owner::type const owner::No_Owner = ETL_NULLPTR;
#endif
  } // namespace timer
} // namespace etl

#endif
