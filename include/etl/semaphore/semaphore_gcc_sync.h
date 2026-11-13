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

#ifndef ETL_SEMAPHORE_GCC_SYNC_INCLUDED
#define ETL_SEMAPHORE_GCC_SYNC_INCLUDED

#include "../platform.h"
#include "../chrono.h"
#include "../limits.h"
#include "semaphore_exceptions.h"

namespace etl
{
  //***************************************************************************
  ///\ingroup semaphore
  ///\brief This counting_semaphore class is implemented using GCC's __atomic functions.
  //***************************************************************************
  template <ptrdiff_t LeastMaxValue = numeric_limits<ptrdiff_t>::max()>
  class counting_semaphore
  {
  public:

    ETL_STATIC_ASSERT(LeastMaxValue >= 0, "LeastMaxValue shall be non-negative");
    ETL_STATIC_ASSERT(LeastMaxValue <= numeric_limits<ptrdiff_t>::max(), "LeastMaxValue is too high");

    ETL_CONSTEXPR14 explicit counting_semaphore(ptrdiff_t desired)
      : counter(desired)
    {
      ETL_ASSERT(desired >= 0 && desired <= max(), ETL_ERROR(semaphore_out_of_bounds));
    }

    void acquire()
    {
      while (!try_acquire())
      {
      }
    }

    bool try_acquire()
    {
      ptrdiff_t old = __atomic_load_n(&counter, __ATOMIC_ACQUIRE);
      if (old <= 0)
      {
        return false;
      }

      return __atomic_compare_exchange_n(&counter, &old, old - 1, false, __ATOMIC_ACQUIRE, __ATOMIC_RELAXED);
    }

    template <class Rep, class Period>
    bool try_acquire_for(const chrono::duration<Rep, Period>& rel_time)
    {
      auto now = chrono::steady_clock::now();
      return try_acquire_until(now + rel_time);
    }

    template <class Clock, class Duration>
    bool try_acquire_until(const chrono::time_point<Clock, Duration>& abs_time)
    {
      bool ret = false;
      while (!(ret = try_acquire()) && Clock::now() < abs_time)
      {
      }
      return ret;
    }

    void release(ptrdiff_t update = 1)
    {
      ptrdiff_t old = __atomic_fetch_add(&counter, update, __ATOMIC_RELEASE);

      // Preconditions are tested afterwards because otherwise it makes the atomic code needlessly complex.
      ETL_ASSERT(update >= 0, ETL_ERROR(semaphore_out_of_bounds));
      ETL_ASSERT(update <= max() - old, ETL_ERROR(semaphore_out_of_bounds));
    }

    static ETL_CONSTEXPR ptrdiff_t max() ETL_NOEXCEPT
    {
      return LeastMaxValue;
    }

  private:

    counting_semaphore(const counting_semaphore&) ETL_DELETE;
    counting_semaphore& operator=(const counting_semaphore&) ETL_DELETE;

    ptrdiff_t counter;
  };

  using binary_semaphore = counting_semaphore<1>;
} // namespace etl

#endif
