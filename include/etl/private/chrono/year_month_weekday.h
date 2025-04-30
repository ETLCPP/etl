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
      year_month_weekday()
        : y()
        , m()
        , wdi()
      {
      }

      //*************************************************************************
      /// Construct from month and day.
      //*************************************************************************
      ETL_CONSTEXPR year_month_weekday(const etl::chrono::year&    y_, 
                                       const etl::chrono::month&   m_, 
                                       const etl::chrono::weekday_indexed& wdi_) ETL_NOEXCEPT
        : y(y_)
        , m(m_)
        , wdi(wdi_)
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
      /// Returns the weekday.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::weekday weekday() const ETL_NOEXCEPT
      {
        return wdi.weekday();
      }

      //*************************************************************************
      /// Returns the weekday index.
      //*************************************************************************
      ETL_CONSTEXPR unsigned index() const ETL_NOEXCEPT
      {
        return wdi.index();
      }

      //*************************************************************************
      /// Returns the weekday_indexed.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::weekday_indexed weekday_indexed() const ETL_NOEXCEPT
      {
        return wdi;
      }

      //*************************************************************************
      /// Returns true if the year/month/day is valid.
      //*************************************************************************
      ETL_CONSTEXPR14 bool ok() const ETL_NOEXCEPT
      {       
        return y.ok() && m.ok() && wdi.ok();
      }

      //*************************************************************************
      /// Adds etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::year_month_weekday& ymwd,
                                                                        const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday(ymwd.year() + dy, ymwd.month(), ymwd.weekday_indexed());
      }

      //*************************************************************************
      /// Adds etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::years& dy,
                                                                        const etl::chrono::year_month_weekday& ymwd) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday(ymwd.year() + dy, ymwd.month(), ymwd.weekday_indexed());
      }

      //*************************************************************************
      /// Adds etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::year_month_weekday& ymwd,
                                                                        const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday(ymwd.year(), ymwd.month() + dm, ymwd.weekday_indexed());
      }

      //*************************************************************************
      /// Adds etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator +(const etl::chrono::months& dm,
                                                                        const etl::chrono::year_month_weekday& ymwd) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday(ymwd.year(), ymwd.month() + dm, ymwd.weekday_indexed());
      }

      //*************************************************************************
      /// Subtracts etl::chrono::years
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator -(const etl::chrono::year_month_weekday& ymwd,
                                                                        const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday(ymwd.year() - dy, ymwd.month(), ymwd.weekday_indexed());
      }

      //*************************************************************************
      /// Subtracts etl::chrono::months
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator -(const etl::chrono::year_month_weekday& ymwd,
                                                                        const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday(ymwd.year(), ymwd.month() - dm, ymwd.weekday_indexed());
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR bool operator ==(const etl::chrono::year_month_weekday& lhs, 
                                            const etl::chrono::year_month_weekday& rhs) ETL_NOEXCEPT
      {
        return (lhs.y == rhs.y) && (lhs.m == rhs.m) && (lhs.wdi == rhs.wdi);
      }

      //*************************************************************************
      /// Inequality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR bool operator !=(const etl::chrono::year_month_weekday& lhs, 
                                            const etl::chrono::year_month_weekday& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

      //***********************************************************************
      /// Converts *this to etl::chrono::sys_days
      //***********************************************************************
      //ETL_CONSTEXPR14 operator etl::chrono::sys_days() const ETL_NOEXCEPT
      //{
      //  // Start with the first day of the given year and month
      //  etl::chrono::year_month_day first_day_of_month(y, m, etl::chrono::day(1));

      //  // Convert the first day of the month to sys_days
      //  etl::chrono::sys_days first_day_sys_days = static_cast<etl::chrono::sys_days>(first_day_of_month);

      //  // Determine the weekday of the first day of the month
      //  etl::chrono::weekday first_weekday = etl::chrono::weekday(first_day_sys_days);

      //  // Calculate the offset to the desired weekday
      //  int offset = (wd.weekday().c_encoding() - first_weekday.c_encoding() + 7) % 7;

      //  // Calculate the day of the month for the desired weekday_indexed
      //  int day_of_month = 1 + offset + (wdi.index() - 1) * 7;

      //  // Ensure the calculated day is valid for the given month
      //  if (day_of_month > etl::chrono::month_day_last(m).day().count())
      //  {
      //    day_of_month = 0; // Invalid day
      //  }

      //  // Create a year_month_day object for the calculated day
      //  etl::chrono::year_month_day ymwd(y, m, etl::chrono::day(day_of_month));

      //  // Convert the year_month_day to sys_days
      //  return static_cast<etl::chrono::sys_days>(ymwd);
      //}

      ////***********************************************************************
      ///// Converts *this to etl::chrono::local_days
      ////***********************************************************************
      //ETL_CONSTEXPR14 explicit operator etl::chrono::local_days() const ETL_NOEXCEPT
      //{
      //  return etl::chrono::local_days();
      //}

    private:

      etl::chrono::year    y;
      etl::chrono::month   m;
      etl::chrono::weekday_indexed wdi;
    };

    //*************************************************************************
    /// year_month_weekday_last
    //*************************************************************************
    class year_month_weekday_last
    {
    public:

      //*************************************************************************
      /// Construct from year, month, weekday_last
      //*************************************************************************
      ETL_CONSTEXPR year_month_weekday_last(const etl::chrono::year&         y_, 
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
      /// Returns the weekday.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::weekday weekday() const ETL_NOEXCEPT
      {
        return wdl.weekday();
      }

      //*************************************************************************
      /// Returns the weekday_last.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::weekday_last weekday_last() const ETL_NOEXCEPT
      {
        return wdl;
      }

      //*************************************************************************
      /// Adds etl::chrono::years.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::year_month_weekday_last& operator +=(const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        y += dy;

        return *this;
      }

      //*************************************************************************
      /// Adds etl::chrono::months.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::year_month_weekday_last& operator +=(const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        m += dm;

        return *this;
      }

      //*************************************************************************
      /// Subtracts etl::chrono::years.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::year_month_weekday_last& operator -=(const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        y -= dy;

        return *this;
      }

      //*************************************************************************
      /// Subtracts etl::chrono::months.
      //*************************************************************************
      ETL_CONSTEXPR etl::chrono::year_month_weekday_last& operator -=(const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        m -= dm;

        return *this;
      }

      //*************************************************************************
      /// Adds etl::chrono::years and const etl::chrono::year_month_weekday_last.
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                             const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday_last(ymwdl.year() + dy, ymwdl.month(), ymwdl.weekday_last());
      }

      //*************************************************************************
      /// Adds etl::chrono::years and const etl::chrono::year_month_weekday_last.
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::years& dy,
                                                                             const etl::chrono::year_month_weekday_last& ymwdl) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday_last(ymwdl.year() + dy, ymwdl.month(), ymwdl.weekday_last());
      }

      //*************************************************************************
      /// Adds const etl::chrono::year_month_weekday_last and etl::chrono::months.
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                             const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday_last(ymwdl.year(), ymwdl.month() + dm, ymwdl.weekday_last());
      }

      //*************************************************************************
      /// Adds etl::chrono::months and const etl::chrono::year_month_weekday_last.
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator +(const etl::chrono::months& dm,
                                                                             const etl::chrono::year_month_weekday_last& ymwdl) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday_last(ymwdl.year(), ymwdl.month() + dm, ymwdl.weekday_last());
      }

      //*************************************************************************
      /// Subtracts etl::chrono::years from const etl::chrono::year_month_weekday_last.
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator -(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                             const etl::chrono::years& dy) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday_last(ymwdl.year() - dy, ymwdl.month(), ymwdl.weekday_last());
      }

      //*************************************************************************
      /// Subtracts etl::chrono::months from const etl::chrono::year_month_weekday_last
      //*************************************************************************
      friend ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator -(const etl::chrono::year_month_weekday_last& ymwdl,
                                                                             const etl::chrono::months& dm) ETL_NOEXCEPT
      {
        return etl::chrono::year_month_weekday_last(ymwdl.year(), ymwdl.month() - dm, ymwdl.weekday_last());
      }

      //*************************************************************************
      /// Equality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR bool operator ==(const etl::chrono::year_month_weekday_last& lhs, 
                                            const etl::chrono::year_month_weekday_last& rhs) ETL_NOEXCEPT
      {
        return (lhs.y == rhs.y) && (lhs.m == rhs.m) && (lhs.weekday() == rhs.weekday());
      }

      //*************************************************************************
      /// Inequality operator.
      //*************************************************************************
      friend ETL_CONSTEXPR bool operator !=(const etl::chrono::year_month_weekday_last& lhs, 
                                            const etl::chrono::year_month_weekday_last& rhs) ETL_NOEXCEPT
      {
        return !(lhs == rhs);
      }

      //***********************************************************************
      /// Spaceship operator
      //***********************************************************************
#if ETL_USING_CPP20
      [[nodiscard]] friend constexpr auto operator <=>(const etl::chrono::year_month_weekday_last& lhs, 
                                                       const etl::chrono::year_month_weekday_last& rhs) ETL_NOEXCEPT
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
            return lhs.weekday().c_encoding() <=> rhs.weekday().c_encoding();
          }
        }
      }
#endif

      //***********************************************************************
      /// Compare year_month_weekday with another.
      /// if year < other.year, returns -1;
      /// else if year > other.year, returns 1;
      /// if month < other.month, returns -1;
      /// else if month > other.month, returns 1;
      /// else returns 0;
      //***********************************************************************
      ETL_CONSTEXPR14 int compare(const year_month_weekday_last& other) const ETL_NOEXCEPT 
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
      //ETL_CONSTEXPR operator etl::chrono::sys_days() const ETL_NOEXCEPT
      //{
      //  etl::chrono::sys_days();
      //}

      //*************************************************************************
      /// 
      //*************************************************************************
      //ETL_CONSTEXPR explicit operator etl::chrono::local_days() const ETL_NOEXCEPT
      //{

      //}

    private:

      etl::chrono::year  y;
      etl::chrono::month m;
      etl::chrono::weekday_last wdl;
    };
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
      uint8_t buffer[sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int)];
      
      unsigned int y = ymwd.year();
      unsigned int m = ymwd.month();
      unsigned int d = ymwd.weekday().c_encoding();

      memcpy(buffer,             &y, sizeof(y));
      memcpy(buffer + sizeof(y), &m, sizeof(m));
      memcpy(buffer + sizeof(y) + sizeof(m), &d, sizeof(d));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int));
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
      uint8_t buffer[sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int)];

      unsigned int y = ymwdl.year();
      unsigned int m = ymwdl.month();
      unsigned int d = ymwdl.weekday_last().weekday().c_encoding();

      memcpy(buffer,             &y, sizeof(y));
      memcpy(buffer + sizeof(y), &m, sizeof(m));
      memcpy(buffer + sizeof(y) + sizeof(m), &d, sizeof(d));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(unsigned int) + sizeof(unsigned int) + sizeof(unsigned int));
    }
  };
#endif
}

