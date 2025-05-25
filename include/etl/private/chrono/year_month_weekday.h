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
    //*************************************************************************
    /// year_month_weekday
    //*************************************************************************
    class year_month_weekday
    {
    public:

      //*************************************************************************
      /// Default constructor.
      //*************************************************************************
      ETL_CONSTEXPR year_month_weekday()
        : y()
        , m()
        , wdi()
      {
      }

      //*************************************************************************
      /// Construct from month, day, and weekday_indexed.
      //*************************************************************************
      ETL_CONSTEXPR14 year_month_weekday(const etl::chrono::year&    y_, 
                                         const etl::chrono::month&   m_, 
                                         const etl::chrono::weekday_indexed& wdi_) ETL_NOEXCEPT
        : y(y_)
        , m(m_)
        , wdi(wdi_)
      {
      }

      //*************************************************************************
      /// Construct from sys_days.
      //*************************************************************************
      ETL_CONSTEXPR14 year_month_weekday(const etl::chrono::sys_days& sd) ETL_NOEXCEPT
      {
        // Extract year, month, and day
        year_month_day ymd = year_month_day{sd};

        etl::chrono::year  yr  = ymd.year();
        etl::chrono::month mth = ymd.month();
        etl::chrono::day   dy  = ymd.day();

        // Get the weekday from sys_days
        etl::chrono::weekday wd = etl::chrono::weekday{sd};

        // Count how many times this weekday has occurred in the month so far
        // We walk backward from the given day in steps of 7 days
        unsigned index = 1;
        
        for (int offset = static_cast<int>(dy) - 7; offset > 0; offset -= 7) 
        {
          ++index;
        }

        y = yr;
        m = mth;
        wdi = etl::chrono::weekday_indexed{ wd, index };
      }

      //*************************************************************************
      /// Construct from local_days.
      //*************************************************************************
      ETL_CONSTEXPR14 year_month_weekday(const etl::chrono::local_days& ld) ETL_NOEXCEPT
      {
        year_month_weekday ymwd(sys_days(ld.time_since_epoch()));

        y   = ymwd.year();
        m   = ymwd.month();
        wdi = ymwd.weekday_indexed();
      }

      //*************************************************************************
      /// Returns the year.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::year year() const ETL_NOEXCEPT
      {
        return y;
      }

      //*************************************************************************
      /// Returns the month.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::month month() const ETL_NOEXCEPT
      {
        return m;
      }

      //*************************************************************************
      /// Returns the weekday.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::weekday weekday() const ETL_NOEXCEPT
      {
        return wdi.weekday();
      }

      //*************************************************************************
      /// Returns the weekday index.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 unsigned index() const ETL_NOEXCEPT
      {
        return wdi.index();
      }

      //*************************************************************************
      /// Returns the weekday_indexed.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::weekday_indexed weekday_indexed() const ETL_NOEXCEPT
      {
        return wdi;
      }

      //*************************************************************************
      /// Returns true if the year/month/day is valid.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {       
        return y.ok() && m.ok() && wdi.ok();
      }

      //***********************************************************************
      /// Converts to etl::chrono::sys_days
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 operator etl::chrono::sys_days() const ETL_NOEXCEPT
      {
        if (ok())
        {
          etl::chrono::year_month_weekday ymwd(*this);
          etl::chrono::year_month_day ymd{ ymwd.year(), ymwd.month(), etl::chrono::day(1) };

          etl::chrono::sys_days sd = ymd;

          unsigned int target_wd = ymwd.weekday().c_encoding();
          unsigned int target_index = ymwd.index();

          etl::chrono::weekday first_weekday(static_cast<int>(sd.time_since_epoch().count()));

          int first_wd = first_weekday.c_encoding();
          int offset = (target_wd - first_wd + 7) % 7;
          int day_of_month = offset + static_cast<int>(target_index - 1) * 7;

          etl::chrono::year_month_day result(year(), month(), etl::chrono::day(day_of_month));

          return etl::chrono::sys_days(result);
        }
        else
        {
          return etl::chrono::sys_days();
        }
      }

      //***********************************************************************
      /// Converts to etl::chrono::local_days
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 operator etl::chrono::local_days() const ETL_NOEXCEPT
      {
        return local_days(sys_days(*this).time_since_epoch());
      }

    private:

      etl::chrono::year    y;
      etl::chrono::month   m;
      etl::chrono::weekday_indexed wdi;
    };

    //*************************************************************************
    /// Adds etl::chrono::years
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::year_month_weekday& ymwd,
                                                                      const etl::chrono::years& dy) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(ymwd.year() + dy, ymwd.month(), ymwd.weekday_indexed());
    }

    //*************************************************************************
    /// Adds etl::chrono::years
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::years& dy,
                                                                      const etl::chrono::year_month_weekday& ymwd) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(ymwd.year() + dy, ymwd.month(), ymwd.weekday_indexed());
    }

    //*************************************************************************
    /// Adds etl::chrono::months
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::year_month_weekday& ymwd,
                                                                      const etl::chrono::months& dm) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(ymwd.year(), ymwd.month() + dm, ymwd.weekday_indexed());
    }

    //*************************************************************************
    /// Adds etl::chrono::months
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::months& dm,
                                                                      const etl::chrono::year_month_weekday& ymwd) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(ymwd.year(), ymwd.month() + dm, ymwd.weekday_indexed());
    }

    //*************************************************************************
    /// Subtracts etl::chrono::years
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator -(const etl::chrono::year_month_weekday& ymwd,
                                                                      const etl::chrono::years& dy) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(ymwd.year() - dy, ymwd.month(), ymwd.weekday_indexed());
    }

    //*************************************************************************
    /// Subtracts etl::chrono::months
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator -(const etl::chrono::year_month_weekday& ymwd,
                                                                      const etl::chrono::months& dm) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(ymwd.year(), ymwd.month() - dm, ymwd.weekday_indexed());
    }

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator ==(const etl::chrono::year_month_weekday& lhs, 
                                            const etl::chrono::year_month_weekday& rhs) ETL_NOEXCEPT
    {
      return (lhs.year() == rhs.year())   && 
             (lhs.month() == rhs.month()) && 
             (lhs.weekday() == rhs.weekday());
    }

    //*************************************************************************
    /// Inequality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator !=(const etl::chrono::year_month_weekday& lhs, 
                                            const etl::chrono::year_month_weekday& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }

    //*************************************************************************
    /// year_month_weekday_last
    //*************************************************************************
    class year_month_weekday_last
    {
    public:

      //*************************************************************************
      /// Construct from year, month, weekday_last
      //*************************************************************************
      ETL_CONSTEXPR14 year_month_weekday_last(const etl::chrono::year&         y_, 
                                              const etl::chrono::month&        m_, 
                                              const etl::chrono::weekday_last& wdl_) ETL_NOEXCEPT
        : y(y_)
        , m(m_)
        , wdl(wdl_)
      {
      }

      //*************************************************************************
      /// Returns the year.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::year year() const ETL_NOEXCEPT
      {
        return y;
      }

      //*************************************************************************
      /// Returns the month.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::month month() const ETL_NOEXCEPT
      {
        return m;
      }

      //*************************************************************************
      /// Returns the weekday.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::weekday weekday() const ETL_NOEXCEPT
      {
        return wdl.weekday();
      }

      //*************************************************************************
      /// Returns the weekday_last.
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::weekday_last weekday_last() const ETL_NOEXCEPT
      {
        return wdl;
      }

      //*************************************************************************
      /// Adds etl::chrono::years.
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last& operator +=(const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        y += dy;

        return *this;
      }

      //*************************************************************************
      /// Adds etl::chrono::months.
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last& operator +=(const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        m += dm;

        return *this;
      }

      //*************************************************************************
      /// Subtracts etl::chrono::years.
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last& operator -=(const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        y -= dy;

        return *this;
      }

      //*************************************************************************
      /// Subtracts etl::chrono::months.
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last& operator -=(const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        m -= dm;

        return *this;
      }

      //*************************************************************************
      /// Converts to etl::chrono::sys_days
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 operator etl::chrono::sys_days() const ETL_NOEXCEPT
      {
        // Get the last day of the month
        etl::chrono::year_month_day_last ymdl(year(), etl::chrono::month_day_last(month()));
        etl::chrono::day last_day = ymdl.day();

        // Walk backward from the last day to find the last occurrence of the target weekday
        unsigned d = static_cast<unsigned>(last_day);
        
        for (; d >= 1; --d)
        {
          etl::chrono::year_month_day ymd(year(), month(), etl::chrono::day(d));
          etl::chrono::sys_days ymd_sys_days = static_cast<etl::chrono::sys_days>(ymd);
          etl::chrono::weekday wd(static_cast<int>(ymd_sys_days.time_since_epoch().count()));
          
          if (wd == weekday())
          {
            return ymd_sys_days;
          }
        }

        // If not found (should not happen for valid input), return epoch
        return etl::chrono::sys_days();
      }

      //*************************************************************************
      /// Converts to etl::chrono::local_days
      //*************************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 explicit operator etl::chrono::local_days() const ETL_NOEXCEPT
      {
        return local_days(sys_days(*this).time_since_epoch());
      }

    private:

      etl::chrono::year  y;
      etl::chrono::month m;
      etl::chrono::weekday_last wdl;
    };

    //*************************************************************************
    /// Adds etl::chrono::years and const etl::chrono::year_month_weekday_last.
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                           const etl::chrono::years& dy) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(ymwdl.year() + dy, ymwdl.month(), ymwdl.weekday_last());
    }

    //*************************************************************************
    /// Adds etl::chrono::years and const etl::chrono::year_month_weekday_last.
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::years& dy,
                                                                           const etl::chrono::year_month_weekday_last& ymwdl) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(ymwdl.year() + dy, ymwdl.month(), ymwdl.weekday_last());
    }

    //*************************************************************************
    /// Adds const etl::chrono::year_month_weekday_last and etl::chrono::months.
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                           const etl::chrono::months& dm) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(ymwdl.year(), ymwdl.month() + dm, ymwdl.weekday_last());
    }

    //*************************************************************************
    /// Adds etl::chrono::months and const etl::chrono::year_month_weekday_last.
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::months& dm,
                                                                           const etl::chrono::year_month_weekday_last& ymwdl) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(ymwdl.year(), ymwdl.month() + dm, ymwdl.weekday_last());
    }

    //*************************************************************************
    /// Subtracts etl::chrono::years from const etl::chrono::year_month_weekday_last.
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator -(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                           const etl::chrono::years& dy) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(ymwdl.year() - dy, ymwdl.month(), ymwdl.weekday_last());
    }

    //*************************************************************************
    /// Subtracts etl::chrono::months from const etl::chrono::year_month_weekday_last
    //*************************************************************************
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator -(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                           const etl::chrono::months& dm) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(ymwdl.year(), ymwdl.month() - dm, ymwdl.weekday_last());
    }

    //*************************************************************************
    /// Equality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator ==(const etl::chrono::year_month_weekday_last& lhs, 
                                            const etl::chrono::year_month_weekday_last& rhs) ETL_NOEXCEPT
    {
      return (lhs.year() == rhs.year())   &&
             (lhs.month() == rhs.month()) &&
             (lhs.weekday() == rhs.weekday());
    }

    //*************************************************************************
    /// Inequality operator.
    //*************************************************************************
    inline ETL_CONSTEXPR14 bool operator !=(const etl::chrono::year_month_weekday_last& lhs, 
                                            const etl::chrono::year_month_weekday_last& rhs) ETL_NOEXCEPT
    {
      return !(lhs == rhs);
    }
  }

  //*************************************************************************
  /// Hash function for etl::chrono::year_month_weekday
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::year_month_weekday>
  {
    size_t operator()(const etl::chrono::year_month_weekday& ymwd) const
    {
      etl::chrono::year::rep  y = static_cast<etl::chrono::year::rep>(static_cast<unsigned>(ymwd.year()));
      etl::chrono::month::rep m = static_cast<etl::chrono::month::rep>(static_cast<unsigned>(ymwd.month()));
      unsigned int  wd = ymwd.weekday().c_encoding();

      uint8_t buffer[sizeof(y) + sizeof(m) + sizeof(wd)];

      memcpy(buffer,                         &y, sizeof(y));
      memcpy(buffer + sizeof(y),             &m, sizeof(m));
      memcpy(buffer + sizeof(y) + sizeof(m), &wd, sizeof(wd));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(y) + sizeof(m) + sizeof(wd));
    }
  };
#endif

  //*************************************************************************
  /// Hash function for etl::chrono::year_month_weekday_last
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::year_month_weekday_last>
  {
    size_t operator()(const etl::chrono::year_month_weekday_last& ymwdl) const
    {
      etl::chrono::year::rep  y = static_cast<etl::chrono::year::rep>(static_cast<unsigned>(ymwdl.year()));
      etl::chrono::month::rep m = static_cast<etl::chrono::month::rep>(static_cast<unsigned>(ymwdl.month()));
      unsigned int  wd = ymwdl.weekday().c_encoding();

      uint8_t buffer[sizeof(y) + sizeof(m) + sizeof(wd)];

      memcpy(buffer,                         &y, sizeof(y));
      memcpy(buffer + sizeof(y),             &m, sizeof(m));
      memcpy(buffer + sizeof(y) + sizeof(m), &wd, sizeof(wd));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(y) + sizeof(m) + sizeof(wd));
    }
  };
#endif
}

