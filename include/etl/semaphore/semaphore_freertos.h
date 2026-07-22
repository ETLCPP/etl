/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2026 Julien Hecht

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

#ifndef ETL_SEMAPHORE_FREERTOS_INCLUDED
#define ETL_SEMAPHORE_FREERTOS_INCLUDED

#include "../platform.h"
#include "../chrono.h"
#include "../error_handler.h"
#include "../limits.h"
#include "semaphore_exceptions.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup semaphore
  ///\brief This counting_semaphore class is implemented using FreeRTOS's semaphores
  //***************************************************************************
  template <ptrdiff_t LeastMaxValue = numeric_limits<BaseType_t>::max()>
  class counting_semaphore
  {
    // On FreeRTOS, LeastMaxValue = 0 causes an assert. This deviates slightly from the C++ spec, but it should be fine.
    ETL_STATIC_ASSERT(LeastMaxValue > 0, "LeastMaxValue should be 1 or more");
    ETL_STATIC_ASSERT(LeastMaxValue <= numeric_limits<BaseType_t>::max(), "LeastMaxValue is too high");

  public:

    explicit counting_semaphore(ptrdiff_t desired)
    {
      ETL_ASSERT(desired >= 0, ETL_ERROR(semaphore_out_of_bounds));
      sem = xSemaphoreCreateCountingStatic(max(), desired, &semaphore_allocation);
    }

    void acquire()
    {
      // portMAX_DELAY=block forever
      // Looping guarantees waiting forever even if the configuration does not have `INCLUDE_vTaskSuspend == 1`
      while (xSemaphoreTake(sem, portMAX_DELAY) != pdTRUE);
    }

    bool try_acquire()
    {
      return try_acquire_for(0_s);
    }

    template <class Rep, class Period>
    bool try_acquire_for(const chrono::duration<Rep, Period>& rel_time)
    {
      using URep_t = make_unsigned_t<Rep>;

      TickType_t           ticks_to_wait = 0;
      chrono::milliseconds ms_to_wait{rel_time};
      if (ms_to_wait > chrono::duration<Rep, Period>::zero())
      {
        ticks_to_wait = divide_round_to_ceiling(static_cast<URep_t>(ms_to_wait.count()), portTICK_PERIOD_MS);
      }
      return xSemaphoreTake(sem, ticks_to_wait) == pdTRUE;
    }

    template <class Clock, class Duration>
    bool try_acquire_until(const chrono::time_point<Clock, Duration>& abs_time)
    {
      return try_acquire_for(abs_time - Clock::now());
    }

    void release(ptrdiff_t update = 1)
    {
      if (update <= 0)
      {
        return;
      }

      // No need to check if the semaphore goes over `max()` because it's guaranteed by FreeRTOS APIs
      while (update--)
      {
        xSemaphoreGive(sem);
      }
    }

    static constexpr ptrdiff_t max() noexcept
    {
      return LeastMaxValue;
    }

  private:

    // Non-copyable
    counting_semaphore(const counting_semaphore&) ETL_DELETE;
    counting_semaphore& operator=(const counting_semaphore&) ETL_DELETE;

    // Memory to hold the semaphore
    StaticSemaphore_t semaphore_allocation;

    // The semaphore handle itself
    SemaphoreHandle_t sem;
  };

  using binary_semaphore = counting_semaphore<1>;
} // namespace etl

#endif
