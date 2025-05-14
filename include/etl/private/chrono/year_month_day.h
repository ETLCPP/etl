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
    /// year_month_day
    //*************************************************************************
    class year_month_day
    {
    public:

      //*************************************************************************
      /// Default constructor.
      //*************************************************************************
      ETL_CONSTEXPR year_month_day()
        : y()
        , m()
        , d()
      {
      }

      //*************************************************************************
      /// Construct from month and day.
      //*************************************************************************
      ETL_CONSTEXPR year_month_day(const etl::chrono::year&  y_, 
                                   const etl::chrono::month& m_, 
                                   const etl::chrono::day&   d_) ETL_NOEXCEPT
        : y(y_)
        , m(m_)
        , d(d_)
      {
      }

      //*************************************************************************
      /// Returns the year.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::year year() const ETL_NOEXCEPT
      {
        return y;
      }

      //*************************************************************************
      /// Returns the month.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::month month() const ETL_NOEXCEPT
      {
        return m;
      }

      //*************************************************************************
      /// Returns the day.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::day day() const ETL_NOEXCEPT
      {
        return d;
      }

      //*************************************************************************
      /// Returns true if the year/month/day is valid.
      //*************************************************************************
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {       
        // Check if the year, month, and day are valid individually
        return y.ok() && 
               m.ok() && 
               d.ok() &&
               d <= max_day_for_month();
      }

      //*************************************************************************
      /// Adds etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day operator +(const etl::chrono::year_month_day& ymd,
                                                                    const etl::chrono::years&          dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day(ymd.year() + dy, ymd.month(), ymd.day());
      }

      //*************************************************************************
      /// Adds etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day operator +(const etl::chrono::years&          dy,
                                                                    const etl::chrono::year_month_day& ymd) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day(ymd.year() + dy, ymd.month(), ymd.day());
      }

      //*************************************************************************
      /// Adds etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day operator +(const etl::chrono::year_month_day& ymd,
                                                                    const etl::chrono::months&         dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day(ymd.year(), ymd.month() + dm, ymd.day());
      }

      //*************************************************************************
      /// Adds etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day operator +(const etl::chrono::months&         dm,
                                                                    const etl::chrono::year_month_day& ymd) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day(ymd.year(), ymd.month() + dm, ymd.day());
      }

      //*************************************************************************
      /// Subtracts etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day operator -(const etl::chrono::year_month_day& ymd,
                                                                    const etl::chrono::years&      dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day(ymd.year() - dy, ymd.month(), ymd.day());
      }

      //*************************************************************************
      /// Subtracts etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day operator -(const etl::chrono::year_month_day& ymd,
                                                                    const etl::chrono::months&     dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day(ymd.year(), ymd.month() - dm, ymd.day());
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator ==(const etl::chrono::year_month_day& lhs, 
                                            const etl::chrono::year_month_day& rhs) ETL_NOEXCEPT
      {
        return (lhs.y == rhs.y) && (lhs.m == rhs.m) && (lhs.d == rhs.d);
      }

      //*************************************************************************
      /// Inequality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator !=(const etl::chrono::year_month_day& lhs, 
                                            const etl::chrono::year_month_day& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

      //***********************************************************************
      /// Spaceship operator
      //***********************************************************************
#if ETL_USING_CPP20
      [[nodiscard]] friend constexpr auto operator <=>(const etl::chrono::year_month_day& lhs, 
                                                       const etl::chrono::year_month_day& rhs) ETL_NOEXCEPT
      {
        auto cmp = lhs.year() <=> rhs.year();

        if (cmp != 0)
        {
          return cmp;
        }

        cmp = lhs.month() <=> rhs.month();

        if (cmp != 0)
        {
          return cmp;
        }

        return lhs.day() <=> rhs.day();
      }
#endif

      //***********************************************************************
      /// Compare year_month_day with another.
      /// if year < other.year, returns -1;
      /// else if year > other.year, returns 1;
      /// if month < other.month, returns -1;
      /// else if month > other.month, returns 1;
      /// else if day < other.day, returns -1;
      /// else if day > other.day, returns 1;
      /// else returns 0;
      //***********************************************************************
      ETL_CONSTEXPR14 int compare(const year_month_day& other) const ETL_NOEXCEPT 
      {
        if (y < other.y) return -1;
        if (y > other.y) return 1;
        if (m < other.m) return -1;
        if (m > other.m) return 1;
        if (d < other.d) return -1;
        if (d > other.d) return 1;
      
        return 0;
      }

      //***********************************************************************
      /// Converts *this to etl::chrono::sys_days
      //***********************************************************************
      ETL_CONSTEXPR14 operator etl::chrono::sys_days() const ETL_NOEXCEPT
      {
        int day_count = 0;

        // Add days for years since 1970
        for (etl::chrono::year yr(1970); yr < this->year(); ++yr)
        {
          day_count += yr.is_leap() ? 366 : 365;
        }

        // Add days for months in the current year
        for (etl::chrono::month mth(1); mth < this->month(); ++mth)
        {
          day_count += private_chrono::days_in_month[mth];
          if (mth == etl::chrono::February && this->year().is_leap())
          {
            ++day_count; // Add one day for leap year February
          }
        }

        // Add days for the current month
        day_count += static_cast<unsigned>(this->day()) - 1;

        return sys_days(etl::chrono::days(day_count));
      }

      ////***********************************************************************
      ///// Converts *this to etl::chrono::local_days
      ////***********************************************************************
      //ETL_CONSTEXPR14 explicit operator etl::chrono::local_days() const ETL_NOEXCEPT
      //{
      //// Convert the year_month_day to sys_days first
      //etl::chrono::sys_days sys_days_representation = static_cast<etl::chrono::sys_days>(*this);

      //// Convert sys_days to local_days (assuming local_days is a wrapper around sys_days)
      //return etl::chrono::local_days(sys_days_representation);
      //}

    private:

      //***********************************************************************
      /// Calculates the last day in the year/month.
      /// Returns 0 if either the year or month are not OK.
      //***********************************************************************
      ETL_CONSTEXPR14 etl::chrono::day max_day_for_month() const
      {
        unsigned char count = 0;

        if (y.ok() && m.ok())
        {
          count = private_chrono::days_in_month[m];

          if (y.is_leap() && (m == February))
          {
            ++count;
          }
        }

        return etl::chrono::day(count);
      }

      etl::chrono::year  y;
      etl::chrono::month m;
      etl::chrono::day   d;
    };

    //*************************************************************************
    /// year_month_day_last
    //*************************************************************************
    class year_month_day_last
    {
    public:

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 year_month_day_last(const etl::chrono::year& y_,
                                        const etl::chrono::month_day_last& mdl_) ETL_NOEXCEPT
        : y(y_)
        , m(mdl_.month())
      {
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year year() const ETL_NOEXCEPT
      {
        return y;
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::month month() const ETL_NOEXCEPT
      {
        return m;
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::day day() const ETL_NOEXCEPT
      {
        etl::chrono::day d = etl::chrono::day(etl::chrono::private_chrono::days_in_month[m]);

        return (d == 28) && y.is_leap() ? etl::chrono::day(29) : d;
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::month_day_last month_day_last() const ETL_NOEXCEPT
      {
        return etl::chrono::month_day_last(m);
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {
        return y.ok() && m.ok();
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_day_last& operator +=(const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        y += dy;

        return *this;
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_day_last& operator +=(const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        m += dm;

        return *this;
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_day_last& operator -=(const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        y -= dy;

        return *this;
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      ETL_CONSTEXPR14 etl::chrono::year_month_day_last& operator -=(const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        m -= dm;

        return *this;
      }

      //*************************************************************************
      /// Adds etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator +(const etl::chrono::year_month_day_last& ymdl,
                                                                         const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day_last(ymdl.year() + dy, ymdl.month_day_last());
      }

      //*************************************************************************
      /// Adds etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator +(const etl::chrono::years& dy,
                                                                         const etl::chrono::year_month_day_last& ymdl) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day_last(ymdl.year() + dy, ymdl.month_day_last());
      }

      //*************************************************************************
      /// Adds etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator +(const etl::chrono::year_month_day_last& ymdl,
                                                                         const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day_last(ymdl.year(), etl::chrono::month_day_last(ymdl.month() + dm));
      }

      //*************************************************************************
      /// Adds etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator +(const etl::chrono::months& dm,
                                                                         const etl::chrono::year_month_day_last& ymdl) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day_last(ymdl.year(), etl::chrono::month_day_last(ymdl.month() + dm));
      }

      //*************************************************************************
      /// Subtracts etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator -(const etl::chrono::year_month_day_last& ymdl,
                                                                         const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day_last(ymdl.year() - dy, ymdl.month_day_last());
      }

      //*************************************************************************
      /// Subtracts etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator -(const etl::chrono::year_month_day_last& ymdl,
                                                                         const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_day_last(ymdl.year(), etl::chrono::month_day_last(ymdl.month() - dm));
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator ==(const etl::chrono::year_month_day_last& lhs, 
                                            const etl::chrono::year_month_day_last& rhs) ETL_NOEXCEPT
      {
        return (lhs.y == rhs.y) && (lhs.m == rhs.m);
      }

      //*************************************************************************
      /// Inequality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR14 bool operator !=(const etl::chrono::year_month_day_last& lhs, 
                                            const etl::chrono::year_month_day_last& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

      //***********************************************************************
      /// Spaceship operator
      //***********************************************************************
#if ETL_USING_CPP20
      [[nodiscard]] friend constexpr auto operator <=>(const etl::chrono::year_month_day_last& lhs, 
                                                       const etl::chrono::year_month_day_last& rhs) ETL_NOEXCEPT
      {
        auto cmp1 = lhs.year() <=> rhs.year();

        if (cmp1 != 0)
        {
          return cmp1;
        }
        else
        {
          auto cmp2 = lhs.month() <=> rhs.month();

          if (cmp2 != 0)
          {
            return cmp2;
          }
          else
          {
            return lhs.month() <=> rhs.month();
          }
        }
      }
#endif

      //***********************************************************************
      /// Compare year_month_day with another.
      /// if year < other.year, returns -1;
      /// else if year > other.year, returns 1;
      /// if month < other.month, returns -1;
      /// else if month > other.month, returns 1;
      /// else returns 0;
      //***********************************************************************
      ETL_CONSTEXPR14 int compare(const year_month_day_last& other) const ETL_NOEXCEPT 
      {
        if (y < other.y) return -1;
        if (y > other.y) return 1;
        if (m < other.m) return -1;
        if (m > other.m) return 1;

        return 0;
      }

      //*************************************************************************
      /// 
      //*************************************************************************
      //ETL_CONSTEXPR14 operator etl::chrono::sys_days() const ETL_NOEXCEPT
      //{
      //  etl::chrono::sys_days();
      //}

      //*************************************************************************
      /// 
      //*************************************************************************
      //ETL_CONSTEXPR14 explicit operator etl::chrono::local_days() const ETL_NOEXCEPT
      //{

      //}

    private:

      etl::chrono::year  y;
      etl::chrono::month m;
    };
  }

  //*************************************************************************
  /// Hash function for etl::chrono::year_month_day
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::year_month_day>
  {
    size_t operator()(const etl::chrono::year_month_day& ymd) const
    {
      uint8_t buffer[sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int)];
      
      unsigned int y = ymd.year();
      unsigned int m = ymd.month();
      unsigned int d = ymd.day();

      memcpy(buffer,             &y, sizeof(y));
      memcpy(buffer + sizeof(y), &m, sizeof(m));
      memcpy(buffer + sizeof(y) + sizeof(m), &d, sizeof(d));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int));
    }
  };
#endif

  //*************************************************************************
  /// Hash function for etl::chrono::year_month_day_last
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <>
  struct hash<etl::chrono::year_month_day_last>
  {
    size_t operator()(const etl::chrono::year_month_day_last& ymdl) const
    {
      uint8_t buffer[sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int)];

      unsigned int y = ymdl.year();
      unsigned int m = ymdl.month();
      unsigned int d = ymdl.day();

      memcpy(buffer,             &y, sizeof(y));
      memcpy(buffer + sizeof(y), &m, sizeof(m));
      memcpy(buffer + sizeof(y) + sizeof(m), &d, sizeof(d));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int));
    }
  };
#endif
}

