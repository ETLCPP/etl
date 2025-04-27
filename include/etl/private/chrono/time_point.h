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
      ETL_CONSTEXPR explicit time_point(const duration& dur_) ETL_NOEXCEPT
        : dur(dur_)
      {
      }

      //***************************************************************************
      /// Copy constructor.
      //***************************************************************************
      ETL_CONSTEXPR time_point(const time_point& rhs) ETL_NOEXCEPT
        : dur(rhs.dur)
      {
      }

      //***************************************************************************
      /// Copy construct from another time_point with a different duration type.
      //***************************************************************************
      template <typename TDuration2>
      explicit time_point(const time_point<TClock, TDuration2>& rhs) ETL_NOEXCEPT
        : dur(rhs.time_since_epoch())
      {
      }

      //***************************************************************************
      /// Assignment operator.
      //***************************************************************************
      time_point& operator =(const time_point& rhs) ETL_NOEXCEPT
      {
        dur = rhs.dur;

        return *this;
      }

      //***************************************************************************
      /// Returns a duration representing the amount of time between this and the clock's epoch.
      //***************************************************************************
      ETL_CONSTEXPR duration time_since_epoch() const ETL_NOEXCEPT
      {
        return dur;
      }

      //***************************************************************************
      /// Adds a duration.
      //***************************************************************************
      time_point& operator +=(const duration& rhs) ETL_NOEXCEPT
      {
        dur += rhs;

        return *this;
      }

      //***************************************************************************
      /// Subtracts a duration.
      //***************************************************************************
      time_point& operator -=(const duration& rhs) ETL_NOEXCEPT
      {
        dur -= rhs;

        return *this;
      }

      //***************************************************************************
      /// Returns a time_point with the smallest possible duration.
      //***************************************************************************
      static ETL_CONSTEXPR time_point min() ETL_NOEXCEPT
      {
        return time_point(duration::min());
      }

      //***************************************************************************
      /// Returns a time_point with the largest possible duration.
      //***************************************************************************
      static ETL_CONSTEXPR time_point max() ETL_NOEXCEPT
      {
        return time_point(duration::max());
      }

    private:

      duration dur;
    };

    //***************************************************************************
    /// Equality operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR bool operator ==(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs)
    {
      return lhs.time_since_epoch() == rhs.time_since_epoch();
    }

    //***************************************************************************
    /// Inequality operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR bool operator !=(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs)
    {
      return !(lhs == rhs);
    }

    //***************************************************************************
    /// Less-than operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR bool operator <(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs)
    {
      return lhs.time_since_epoch() < rhs.time_since_epoch();
    }

    //***************************************************************************
    /// Less-than-equal operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR bool operator <=(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs)
    {
      return !(rhs < lhs);
    }

    //***************************************************************************
    /// Greater-than operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR bool operator >(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs)
    {
      return rhs < lhs;
    }

    //***************************************************************************
    /// Greater-than-equal operator
    //***************************************************************************
    template <typename TClock, typename TDuration1, typename TDuration2>
    ETL_CONSTEXPR bool operator >=(const time_point<TClock, TDuration1>& lhs, const time_point<TClock, TDuration2>& rhs)
    {
      return !(lhs < rhs);
    }

    //***************************************************************************
    /// Local time
    //***************************************************************************
    //struct local_t 
    //{
    //  using rep        = size_t;
    //  using period     = etl::chrono::seconds;
    //  using duration   = etl::chrono::duration<rep, period>;
    //  using time_point = etl::chrono::time_point<local_t>;

    //  const bool is_steady = true;

    //  time_point now()
    //  {
    //    return time_point();
    //  }
    //};

    //template <typename TDuration>
    //using local_time    = etl::chrono::time_point<etl::chrono::local_t, TDuration>;

    //using local_seconds = local_time<etl::chrono::seconds>;
    //using local_days    = local_time<etl::chrono::days>;
  }

  //***************************************************************************
  /// Defines type, which is the common type of two std::chrono::time_points.
  //***************************************************************************
  template <typename TClock, typename TDuration1, typename TDuration2>
  struct common_type<etl::chrono::time_point<TClock, TDuration1>, etl::chrono::time_point<TClock, TDuration2>>
  {
    using type = etl::chrono::time_point<TClock, typename etl::common_type<TDuration1, TDuration2>::type>;
  };
}
