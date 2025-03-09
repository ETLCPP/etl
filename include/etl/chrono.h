/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 BMW AG

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

#include "platform.h"
#include "limits.h"
#include "ratio.h"
#include "stdint.h"
#include "type_traits.h"

//*****************************************************************************
///\defgroup chrono chrono
/// Provides functionalities of std::chrono
///\ingroup utilities
//*****************************************************************************
namespace etl
{
  namespace chrono
  {
    template <typename Rep, typename Period = ratio<1>>
    struct duration;

    template <typename Clock, typename Dur = typename Clock::duration>
    struct time_point;

    namespace private_chrono
    {
      template <typename T>
      struct is_duration_impl : false_type
      {
      };

      template <typename Rep, typename Period>
      struct is_duration_impl<duration<Rep, Period>> : true_type
      {
      };

      template <typename T>
      struct is_duration : is_duration_impl<typename decay<T>::type>
      {
      };

      template <typename FromDuration,
                typename ToDuration,
                typename Period = ratio_divide<typename FromDuration::period,
                                               typename ToDuration::period>,
                bool = (Period::num == 1),
                bool = (Period::den == 1)>
      struct duration_cast_impl;

      template <typename FromDuration, typename ToDuration, typename Period>
      struct duration_cast_impl<FromDuration, ToDuration, Period, true, true>
      {
        static ETL_CONSTEXPR ToDuration cast(const FromDuration& dur)
        {
          return ToDuration(static_cast<typename ToDuration::rep>(dur.count()));
        }
      };

      template <typename FromDuration, typename ToDuration, typename Period>
      struct duration_cast_impl<FromDuration, ToDuration, Period, true, false>
      {
        static ETL_CONSTEXPR ToDuration cast(const FromDuration& dur)
        {
          using Ct = typename common_type<typename ToDuration::rep, typename FromDuration::rep, intmax_t>::type;
          return ToDuration(
            static_cast<typename ToDuration::rep>(static_cast<Ct>(dur.count()) / static_cast<Ct>(Period::den)));
        }
      };

      template <typename FromDuration, typename ToDuration, typename Period>
      struct duration_cast_impl<FromDuration, ToDuration, Period, false, true>
      {
        static ETL_CONSTEXPR ToDuration cast(const FromDuration& dur)
        {
          using Ct = typename common_type<typename ToDuration::rep, typename FromDuration::rep, intmax_t>::type;
          return ToDuration(
            static_cast<typename ToDuration::rep>(static_cast<Ct>(dur.count()) * static_cast<Ct>(Period::num)));
        }
      };

      template <typename FromDuration, typename ToDuration, typename Period>
      struct duration_cast_impl<FromDuration, ToDuration, Period, false, false>
      {
        static ETL_CONSTEXPR ToDuration cast(const FromDuration& dur)
        {
          using Ct = typename common_type<typename ToDuration::rep, typename FromDuration::rep, intmax_t>::type;
          return ToDuration(static_cast<typename ToDuration::rep>(
            static_cast<Ct>(dur.count()) * static_cast<Ct>(Period::num) / static_cast<Ct>(Period::den)));
        }
      };
    }  // namespace private_chrono

    template <typename ToDuration, typename Rep, typename Period, typename enable_if<private_chrono::is_duration<ToDuration>::value, int>::type = 0>
    ETL_CONSTEXPR ToDuration duration_cast(const duration<Rep, Period>& dur)
    {
      return private_chrono::duration_cast_impl<duration<Rep, Period>, ToDuration>::cast(dur);
    }
  }  // namespace chrono

  template <typename Rep1, typename Period1, typename Rep2, typename Period2>
  struct common_type<chrono::duration<Rep1, Period1>, chrono::duration<Rep2, Period2>>
  {
    using type = chrono::duration<typename common_type<Rep1, Rep2>::type,
                                  ratio<
                                    private_ratio::ratio_gcd<intmax_t, Period1::num, Period2::num>::value,
                                    private_ratio::ratio_lcm<intmax_t, Period1::den, Period2::den>::value>>;
  };

  template <typename Clock, typename Dur1, typename Dur2>
  struct common_type<chrono::time_point<Clock, Dur1>, chrono::time_point<Clock, Dur2>>
  {
    using type = chrono::time_point<Clock, typename common_type<Dur1, Dur2>::type>;
  };

  namespace chrono
  {
    template <typename Rep>
    struct duration_values
    {
      static Rep ETL_CONSTEXPR zero() ETL_NOEXCEPT
      {
        return Rep(0);
      };

      static Rep ETL_CONSTEXPR min() ETL_NOEXCEPT
      {
        return numeric_limits<Rep>::lowest();
      }

      static Rep ETL_CONSTEXPR max() ETL_NOEXCEPT
      {
        return numeric_limits<Rep>::max();
      }
    };

    template <typename Rep, typename Period>
    struct duration
    {
      using rep = Rep;
      using period = Period;

      ETL_CONSTEXPR duration()
        : val(0)
      {
      }

      duration(duration const& rhs)
        : val(rhs.val)
      {
      }

      template <typename Rep2, typename enable_if<is_convertible<const Rep2&, rep>::value && (is_floating_point<rep>::value || !is_floating_point<Rep2>::value), int>::type = 0>
      ETL_CONSTEXPR explicit duration(Rep2 const& rhs)
        : val(rhs)
      {
      }

      template <typename Rep2, typename Period2>
      ETL_CONSTEXPR duration(duration<Rep2, Period2> const& dur)
        : val(
            duration_cast<
              typename enable_if<(ratio_divide<Period2, Period>::den == 1), duration<Rep, Period>>::type>(
              dur)
              .count())
      {
      }

      duration& operator=(duration const& rhs)
      {
        val = rhs.val;
        return *this;
      }

      ETL_CONSTEXPR rep count() const
      {
        return val;
      }

      ETL_CONSTEXPR duration operator+() const
      {
        return *this;
      }

      ETL_CONSTEXPR duration operator-() const
      {
        return duration(-val);
      }

      duration& operator++()
      {
        ++val;
        return *this;
      }

      duration operator++(int)
      {
        duration ret = *this;
        ++val;
        return ret;
      }

      duration& operator--()
      {
        --val;
        return *this;
      }

      duration operator--(int)
      {
        duration ret = *this;
        --val;
        return ret;
      }

      duration& operator+=(duration const& rhs)
      {
        val += rhs.count();
        return *this;
      }

      duration& operator-=(duration const& rhs)
      {
        val -= rhs.count();
        return *this;
      }

      duration& operator*=(rep const& r)
      {
        val *= r;
        return *this;
      }

      duration& operator/=(rep const& rhs)
      {
        val /= rhs;
        return *this;
      }

      duration& operator%=(rep const& rhs)
      {
        val %= rhs;
        return *this;
      }

      duration& operator%=(duration const& rhs)
      {
        val %= rhs.count();
        return *this;
      }

      static ETL_CONSTEXPR duration zero() ETL_NOEXCEPT
      {
        return duration<Rep, Period>(duration_values<Rep>::zero());
      }

      static ETL_CONSTEXPR duration min() ETL_NOEXCEPT
      {
        return duration<Rep, Period>(duration_values<Rep>::min());
      }

      static ETL_CONSTEXPR duration max() ETL_NOEXCEPT
      {
        return duration<Rep, Period>(duration_values<Rep>::max());
      }

    private:
      Rep val;
    };

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
    operator+(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using cd_t = typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
      return cd_t(cd_t(lhs).count() + cd_t(rhs).count());
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
    operator-(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using cd_t = typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
      return cd_t(cd_t(lhs).count() - cd_t(rhs).count());
    }

    template <typename Rep1, typename Period, typename Rep2>
    duration<typename common_type<Rep1, Rep2>::type, Period>
    operator*(duration<Rep1, Period> const& d, Rep2 const& r)
    {
      using cd_t = duration<typename common_type<Rep1, Rep2>::type, Period>;
      return cd_t(cd_t(d).count() * r);
    }

    template <typename Rep1, typename Rep2, typename Period>
    duration<typename common_type<Rep2, Rep1>::type, Period>
    operator*(Rep1 const& r, duration<Rep2, Period> const& d)
    {
      return d * r;
    }

    template <typename Rep1, typename Period, typename Rep2>
    duration<typename common_type<Rep1, Rep2>::type, Period>
    operator/(duration<Rep1, Period> const& d, Rep2 const& r)
    {
      using cd_t = duration<typename common_type<Rep1, Rep2>::type, Period>;
      return cd_t(cd_t(d).count() / r);
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    typename common_type<Rep1, Rep2>::type
    operator/(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using cd_t = typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
      return cd_t(lhs).count() / cd_t(rhs).count();
    }

    template <typename Rep1, typename Period, typename Rep2>
    duration<typename common_type<Rep1, Rep2>::type, Period>
    operator%(duration<Rep1, Period> const& d, Rep2 const& r)
    {
      using cd_t = duration<typename common_type<Rep1, Rep2>::type, Period>;
      return cd_t(cd_t(d).count() % r);
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type
    operator%(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using cd_t = typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
      return cd_t(cd_t(lhs).count() % cd_t(rhs).count());
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    bool operator==(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using cd_t = typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
      return cd_t(lhs).count() == cd_t(rhs).count();
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    bool operator!=(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      return !(lhs == rhs);
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    bool operator<(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using cd_t = typename common_type<duration<Rep1, Period1>, duration<Rep2, Period2>>::type;
      return cd_t(lhs).count() < cd_t(rhs).count();
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    bool operator<=(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      return !(rhs < lhs);
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    bool operator>(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      return rhs < lhs;
    }

    template <typename Rep1, typename Period1, typename Rep2, typename Period2>
    bool operator>=(duration<Rep1, Period1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      return !(lhs < rhs);
    }

    using nanoseconds = duration<int64_t, nano>;
    using microseconds = duration<int64_t, micro>;
    using milliseconds = duration<int64_t, milli>;
    using seconds = duration<int64_t>;
    using minutes = duration<int32_t, ratio<60>>;
    using hours = duration<int32_t, ratio<3600>>;
    template <typename Clock, typename Dur>
    struct time_point
    {
      using clock = Clock;
      using duration = Dur;
      using rep = typename duration::rep;
      using period = typename duration::period;

      ETL_CONSTEXPR time_point()
        : dur(duration::zero())
      {
      }

      ETL_CONSTEXPR explicit time_point(duration const& d)
        : dur(d)
      {
      }

      ETL_CONSTEXPR time_point(time_point const& rhs)
        : dur(rhs.dur)
      {
      }

      template <typename Dur2>
      explicit time_point(time_point<Clock, Dur2> const& rhs)
        : dur(rhs.time_since_epoch())
      {
      }

      time_point& operator=(time_point const& rhs)
      {
        dur = rhs.dur;
        return *this;
      }

      ETL_CONSTEXPR duration time_since_epoch() const
      {
        return dur;
      }

      time_point& operator+=(duration const& rhs)
      {
        dur += rhs;
        return *this;
      }

      time_point& operator-=(duration const& rhs)
      {
        dur -= rhs;
        return *this;
      }

      static ETL_CONSTEXPR time_point min() ETL_NOEXCEPT
      {
        return time_point(duration::min());
      }

      static ETL_CONSTEXPR time_point max() ETL_NOEXCEPT
      {
        return time_point(duration::max());
      }

    private:
      duration dur;
    };

    template <typename ToDur, typename Clock, typename Dur>
    time_point<Clock, ToDur> time_point_cast(time_point<Clock, Dur> const& tp)
    {
      using time_point_t =
        typename enable_if<private_chrono::is_duration<ToDur>::value, time_point<Clock, ToDur>>::type;
      return time_point_t(duration_cast<ToDur>(tp.time_since_epoch()));
    }

    template <typename Clock, typename Dur1, typename Rep2, typename Period2>
    time_point<Clock, typename common_type<Dur1, duration<Rep2, Period2>>::type>
    operator+(time_point<Clock, Dur1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using ct_t = typename common_type<Dur1, duration<Rep2, Period2>>::type;
      using time_point_t = time_point<Clock, ct_t>;
      return time_point_t(lhs.time_since_epoch() + rhs);
    }

    template <typename Rep1, typename Period1, typename Clock, typename Dur2>
    time_point<Clock, typename common_type<duration<Rep1, Period1>, Dur2>::type>
    operator+(duration<Rep1, Period1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return rhs + lhs;
    }

    template <typename Clock, typename Dur1, typename Rep2, typename Period2>
    time_point<Clock, typename common_type<Dur1, duration<Rep2, Period2>>::type>
    operator-(time_point<Clock, Dur1> const& lhs, duration<Rep2, Period2> const& rhs)
    {
      using ct_t = typename common_type<Dur1, duration<Rep2, Period2>>::type;
      using time_point_t = time_point<Clock, ct_t>;
      return time_point_t(lhs.time_since_epoch() - rhs);
    }

    template <typename Clock, typename Dur1, typename Dur2>
    typename common_type<Dur1, Dur2>::type
    operator-(time_point<Clock, Dur1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return lhs.time_since_epoch() - rhs.time_since_epoch();
    }

    //***************************************************************************
    ///\brief C function supporting etl::chrono::high_resolution_clock
    ///
    /// Users of etl::chrono::high_resolution_clock should provide the definition
    /// of this function.
    /// etl::chrono::high_resolution_clock depends on it to get time points.
    /// It should model a std::chrono::steady_clock .
    ///
    ///\return current time in nanosecond
    //***************************************************************************
    extern "C" uint64_t etl_get_system_time_nanosec();

    class high_resolution_clock
    {
    public:
      using duration = nanoseconds;
      using rep = duration::rep;
      using period = duration::period;
      using time_point = chrono::time_point<high_resolution_clock, duration>;

      static bool ETL_CONSTANT is_steady = true;

      static time_point now() ETL_NOEXCEPT
      {
        return time_point(duration(static_cast<int64_t>(etl_get_system_time_nanosec())));
      }
    };

    template <typename Clock, typename Dur1, typename Dur2>
    bool operator==(time_point<Clock, Dur1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return lhs.time_since_epoch() == rhs.time_since_epoch();
    }

    template <typename Clock, typename Dur1, typename Dur2>
    bool operator!=(time_point<Clock, Dur1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return !(lhs == rhs);
    }

    template <typename Clock, typename Dur1, typename Dur2>
    bool operator<(time_point<Clock, Dur1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return lhs.time_since_epoch() < rhs.time_since_epoch();
    }

    template <typename Clock, typename Dur1, typename Dur2>
    bool operator<=(time_point<Clock, Dur1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return !(rhs < lhs);
    }

    template <typename Clock, typename Dur1, typename Dur2>
    bool operator>(time_point<Clock, Dur1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return rhs < lhs;
    }

    template <typename Clock, typename Dur1, typename Dur2>
    bool operator>=(time_point<Clock, Dur1> const& lhs, time_point<Clock, Dur2> const& rhs)
    {
      return !(lhs < rhs);
    }
  }  // namespace chrono
}  // namespace etl

#endif
