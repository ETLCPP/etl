///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2025 John Wellbelove

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

#ifndef ETL_IN_CHRONO_H
  #error DO NOT DIRECTLY INCLUDE THIS FILE. USE CHRONO.H
#endif

namespace etl
{
  namespace chrono
  {
    //***************************************************************************
    /// Represents a point in time storing a TDuration indicating the time
    /// interval from the start of the TClock's epoch.
    //***************************************************************************
    template <typename TClock, typename TDuration = typename TClock::duration>
    class time_point
    {
    public:

      using clock    = TClock;
      using duration = TDuration;
      using rep      = typename TDuration::rep;
      using period   = typename TDuration::period;

      //***************************************************************************
      /// Default constructor.
      //***************************************************************************
      ETL_CONSTEXPR time_point() ETL_NOEXCEPT
        : dur(duration::zero())
      {
      }

      //***************************************************************************
      /// Construct from a duration.
      //***************************************************************************
      ETL_CONSTEXPR14 explicit time_point(const duration& dur_) ETL_NOEXCEPT
        : dur(dur_)
      {
      }

      //***************************************************************************
      /// Copy constructor.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point(const time_point& rhs) ETL_NOEXCEPT
        : dur(rhs.dur)
      {
      }

      //***************************************************************************
      /// Copy construct from another time_point with a different duration type.
      //***************************************************************************
      template <typename TDuration2>
      ETL_CONSTEXPR14 explicit time_point(const time_point<clock, TDuration2>& rhs) ETL_NOEXCEPT
        : dur(rhs.time_since_epoch())
      {
      }

      //***************************************************************************
      /// Assignment operator.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point& operator=(const time_point& rhs) ETL_NOEXCEPT
      {
        dur = rhs.dur;

        return *this;
      }

      //***************************************************************************
      /// Returns a duration representing the amount of time between this and
      /// the clock's epoch.
      //***************************************************************************
      ETL_NODISCARD ETL_CONSTEXPR14 duration time_since_epoch() const ETL_NOEXCEPT
      {
        return dur;
      }

      //***************************************************************************
      /// Adds a duration.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point& operator+=(const duration& rhs) ETL_NOEXCEPT
      {
        dur += rhs;

        return *this;
      }

      //***************************************************************************
      /// Subtracts a duration.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point& operator-=(const duration& rhs) ETL_NOEXCEPT
      {
        dur -= rhs;

        return *this;
      }

      //***************************************************************************
      /// Pre-increments the stored duration by one tick.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point& operator++() ETL_NOEXCEPT
      {
        ++dur;

        return *this;
      }

      //***************************************************************************
      /// Post-increments the stored duration by one tick.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point operator++(int) ETL_NOEXCEPT
      {
        time_point temp(*this);
        ++dur;

        return temp;
      }

      //***************************************************************************
      /// Pre-decrements the stored duration by one tick.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point& operator--() ETL_NOEXCEPT
      {
        --dur;

        return *this;
      }

      //***************************************************************************
      /// Post-decrements the stored duration by one tick.
      //***************************************************************************
      ETL_CONSTEXPR14 time_point operator--(int) ETL_NOEXCEPT
      {
        time_point temp(*this);
        --dur;

        return temp;
      }

      //***************************************************************************
      /// Returns a time_point with the smallest possible duration.
      //***************************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 time_point min() ETL_NOEXCEPT
      {
        return time_point(duration::min());
      }

      //***************************************************************************
      /// Returns a time_point with the largest possible duration.
      //***************************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 time_point max() ETL_NOEXCEPT
      {
        return time_point(duration::max());
      }

      //***********************************************************************
      /// Compare day with another.
      /// if time_point < other, returns -1
      /// else if time_point > other, returns 1
      /// else returns 0
      //***********************************************************************
      ETL_NODISCARD ETL_CONSTEXPR14 int compare(const time_point& other) const ETL_NOEXCEPT
      {
        if (dur < other.dur)
          return -1;
        if (dur > other.dur)
          return 1;

        return 0;
      }

    private:

      duration dur;
    };

    //***********************************************************************
    /// Rounds down a duration to the nearest lower precision.
    //***********************************************************************
    template <typename TToDuration, typename TClock, typename TDuration>
    ETL_NODISCARD ETL_CONSTEXPR14 etl::chrono::time_point<TClock, TToDuration> floor(const etl::chrono::time_point<TClock, TDuration>& tp)
      ETL_NOEXCEPT
    {
      return etl::chrono::time_point<TClock, TToDuration>(floor<TToDuration>(tp.time_since_epoch()));
    }

    //***********************************************************************
    /// Rounds up a duration to the nearest higher precision.
    //***********************************************************************
    template <typename TToDuration, typename TClock, typename TDuration>
    ETL_NODISCARD ETL_CONSTEXPR14 etl::chrono::time_point<TClock, TToDuration> ceil(const etl::chrono::time_point<TClock, TDuration>& tp) ETL_NOEXCEPT
    {
      return etl::chrono::time_point<TClock, TToDuration>(ceil<TToDuration>(tp.time_since_epoch()));
    }

    //***********************************************************************
    /// Rounds a duration to the nearest precision.
    /// If the duration is exactly halfway, it rounds away from zero.
    //***********************************************************************
    template <typename TToDuration, typename TClock, typename TDuration>
    ETL_NODISCARD ETL_CONSTEXPR14 etl::chrono::time_point<TClock, TToDuration> round(const etl::chrono::time_point<TClock, TDuration>& tp)
      ETL_NOEXCEPT
    {
      return etl::chrono::time_point<TClock, TToDuration>(round<TToDuration>(tp.time_since_epoch()));
    }

    template <typename TToDuration, typename TClock, typename TDuration>
    ETL_NODISCARD ETL_CONSTEXPR14 etl::chrono::time_point<TClock, TToDuration> time_point_cast(const etl::chrono::time_point<TClock, TDuration>& tp)
      ETL_NOEXCEPT
    {
      TToDuration dur = etl::chrono::duration_cast<TToDuration>(tp.time_since_epoch());

      return etl::chrono::time_point<TClock, TToDuration>(dur);
    }

    //***************************************************************************
    /// Adds a duration to a time_point.
    /// Returns a time_point whose duration is the common type of the two.
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TRep2, typename TPeriod2>
    ETL_CONSTEXPR14 etl::chrono::time_point<TClock, typename etl::common_type<TDuration1, etl::chrono::duration<TRep2, TPeriod2> >::type>
      operator+(const time_point<TClock, TDuration1>& lhs, const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
    {
      using common_duration   = typename etl::common_type<TDuration1, etl::chrono::duration<TRep2, TPeriod2> >::type;
      using result_time_point = etl::chrono::time_point<TClock, common_duration>;

      return result_time_point(lhs.time_since_epoch() + rhs);
    }

    //***************************************************************************
    /// Adds a time_point to a duration.
    /// Returns a time_point whose duration is the common type of the two.
    //***************************************************************************
    template <typename TRep1, typename TPeriod1, typename TClock, typename TDuration2>
    ETL_CONSTEXPR14 etl::chrono::time_point<TClock, typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, TDuration2>::type>
      operator+(const etl::chrono::duration<TRep1, TPeriod1>& lhs, const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      using common_duration   = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, TDuration2>::type;
      using result_time_point = etl::chrono::time_point<TClock, common_duration>;

      return result_time_point(lhs + rhs.time_since_epoch());
    }

    //***************************************************************************
    /// Subtracts a duration from a time_point.
    /// Returns a time_point whose duration is the common type of the two.
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TRep2, typename TPeriod2>
    ETL_CONSTEXPR14 etl::chrono::time_point<TClock, typename etl::common_type<TDuration1, etl::chrono::duration<TRep2, TPeriod2> >::type>
      operator-(const time_point<TClock, TDuration1>& lhs, const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
    {
      using common_duration   = typename etl::common_type<TDuration1, etl::chrono::duration<TRep2, TPeriod2> >::type;
      using result_time_point = etl::chrono::time_point<TClock, common_duration>;

      return result_time_point(lhs.time_since_epoch() - rhs);
    }

    //***************************************************************************
    /// Subtracts one time_point from another.
    /// Returns the duration between them as the common type of the two durations.
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR14 typename etl::common_type<TDuration1, TDuration2>::type operator-(const time_point<TClock, TDuration1>& lhs,
                                                                                const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      return lhs.time_since_epoch() - rhs.time_since_epoch();
    }

    //***************************************************************************
    /// Equality operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR14 bool operator==(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      return lhs.time_since_epoch() == rhs.time_since_epoch();
    }

    //***************************************************************************
    /// Inequality operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR14 bool operator!=(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //***************************************************************************
    /// Less-than operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR14 bool operator<(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      return lhs.time_since_epoch() < rhs.time_since_epoch();
    }

    //***************************************************************************
    /// Less-than-equal operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR14 bool operator<=(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      return !(rhs < lhs);
    }

    //***************************************************************************
    /// Greater-than operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR14 bool operator>(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      return rhs < lhs;
    }

    //***************************************************************************
    /// Greater-than-equal operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR14 bool operator>=(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
    {
      return !(lhs < rhs);
    }
  } // namespace chrono

  //***********************************************************************
  /// Spaceship operator
  //***********************************************************************
#if ETL_USING_CPP20
  template <typename TClock, typename TDuration1, typename TDuration2>
  [[nodiscard]]
  constexpr auto operator<=>(const etl::chrono::time_point<TClock, TDuration1>& lhs, const etl::chrono::time_point<TClock, TDuration2>& rhs)
    ETL_NOEXCEPT
  {
    return (lhs.time_since_epoch() <=> rhs.time_since_epoch());
  }
#endif

  //***************************************************************************
  /// Defines type, which is the common type of two etl::chrono::time_points.
  //***************************************************************************
  template <typename TClock, typename TDuration1, typename TDuration2>
  struct common_type<etl::chrono::time_point<TClock, TDuration1>, etl::chrono::time_point<TClock, TDuration2>>
  {
    using type = etl::chrono::time_point< TClock, typename etl::common_type<TDuration1, TDuration2>::type>;
  };
} // namespace etl
