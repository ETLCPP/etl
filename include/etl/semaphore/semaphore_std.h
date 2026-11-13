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

#ifndef ETL_SEMAPHORE_STD_INCLUDED
#define ETL_SEMAPHORE_STD_INCLUDED

#include "../platform.h"
#include "../chrono.h"

#include <chrono>
#include <cstddef>
#include <semaphore>

namespace etl
{
  //***************************************************************************
  ///\ingroup semaphore
  ///\brief These semaphore classes are implemented using std::counting_semaphore and std::binary_semaphore.
  //***************************************************************************

  namespace private_semaphore_std
  {
    template <class Clock>
    struct compat_clock
    {
      using rep                   = typename Clock::rep;
      using period                = std::ratio<Clock::period::num, Clock::period::den>;
      using duration              = std::chrono::duration<rep, period>;
      using time_point            = std::chrono::time_point<compat_clock>;
      static const bool is_steady = Clock::is_steady;
      static time_point now()
      {
        return time_point{duration{Clock::now().time_since_epoch().count()}};
      }
    };
  } // namespace private_semaphore_std

  template <ptrdiff_t LeastMaxValue = std::counting_semaphore<>::max()>
  class counting_semaphore
  {
    ETL_STATIC_ASSERT(LeastMaxValue > 0, "LeastMaxValue should be 1 or more");
    ETL_STATIC_ASSERT(LeastMaxValue <= std::counting_semaphore<>::max(), "LeastMaxValue is too high");

  public:

    explicit counting_semaphore(ptrdiff_t desired)
      : sem(desired)
    {
    }

    void acquire()
    {
      return sem.acquire();
    }

    bool try_acquire()
    {
      return sem.try_acquire();
    }

    template <class Rep, class Period>
    bool try_acquire_for(const etl::chrono::duration<Rep, Period>& rel_time)
    {
      std::chrono::duration<Rep, std::ratio<Period::num, Period::den>> duration(rel_time.count());

      return sem.try_acquire_for(duration);
    }

    template <class Clock, class Duration>
    bool try_acquire_until(const etl::chrono::time_point<Clock, Duration>& abs_time)
    {
      using CompatDuration = std::chrono::duration<typename Duration::rep, std::ratio<Duration::period::num, Duration::period::den>>;

      std::chrono::time_point<private_semaphore_std::compat_clock<Clock>, CompatDuration> time_point{
        CompatDuration(abs_time.time_since_epoch().count())};

      return sem.try_acquire_until(time_point);
    }

    void release(ptrdiff_t update = 1)
    {
      return sem.release(update);
    }

    static constexpr ptrdiff_t max() noexcept
    {
      return LeastMaxValue;
    }

  private:

    // Non-copyable
    counting_semaphore(const counting_semaphore&) ETL_DELETE;
    counting_semaphore& operator=(const counting_semaphore&) ETL_DELETE;

    std::counting_semaphore<LeastMaxValue> sem;
  };

  using binary_semaphore = counting_semaphore<1>;
} // namespace etl

#endif
