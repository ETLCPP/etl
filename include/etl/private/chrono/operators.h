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
    // month_day
    inline ETL_CONSTEXPR14 etl::chrono::month_day operator /(const etl::chrono::month& m,
                                                             const etl::chrono::day& d) ETL_NOEXCEPT
    {
      return etl::chrono::month_day(m, d);
    }

    // month_day
    inline ETL_CONSTEXPR14 etl::chrono::month_day operator /(const etl::chrono::month& m, 
                                                             int d) ETL_NOEXCEPT
    {
      return etl::chrono::month_day(m, etl::chrono::day(d));
    }

    // month_day
    inline ETL_CONSTEXPR14 etl::chrono::month_day operator /(int m, 
                                                             const etl::chrono::day& d) ETL_NOEXCEPT
    {
      return etl::chrono::month_day(etl::chrono::month(m), d);
    }

    // month_day
    inline ETL_CONSTEXPR14 etl::chrono::month_day operator /(const etl::chrono::day& d, 
                                                             const etl::chrono::month& m) ETL_NOEXCEPT
    {
      return etl::chrono::month_day(m, d);
    }

    // month_day
    inline ETL_CONSTEXPR14 etl::chrono::month_day operator /(const etl::chrono::day& d, 
                                                             int m) ETL_NOEXCEPT
    {
      return etl::chrono::month_day(etl::chrono::month(m), d);
    }

    //*************************************************************************
    // month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::month_day_last operator /(const etl::chrono::month& m,
                                                                  etl::chrono::last_spec) ETL_NOEXCEPT
    {
      return etl::chrono::month_day_last(m);
    }

    // month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::month_day_last operator /(int m, 
                                                                  etl::chrono::last_spec) ETL_NOEXCEPT
    {
      return etl::chrono::month_day_last(etl::chrono::month(m));
    }

    // month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::month_day_last operator /(etl::chrono::last_spec,
                                                                  const etl::chrono::month& m) ETL_NOEXCEPT
    {
      return etl::chrono::month_day_last(m);
    }
     
    // month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::month_day_last operator/(etl::chrono::last_spec, 
                                                                 int m) ETL_NOEXCEPT
    {
      return etl::chrono::month_day_last(etl::chrono::month(m));
    }

    //*************************************************************************
    // month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday operator /(const etl::chrono::month& m,
                                                                 const etl::chrono::weekday_indexed& wdi) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday(m, wdi);
    }

    // month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday operator /(int m, 
                                                                 const etl::chrono::weekday_indexed& wdi) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday(etl::chrono::month(m), wdi);
    }

    // month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday operator /(const etl::chrono::weekday_indexed& wdi,
                                                                 const etl::chrono::month& m) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday(m, wdi);
    }

    // month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday operator /(const etl::chrono::weekday_indexed& wdi, 
                                                                 int m) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday(etl::chrono::month(m), wdi);
    }

    //*************************************************************************
    // month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday_last operator /(const etl::chrono::month& m,
                                                                      const etl::chrono::weekday_last& wdl) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday_last(m, wdl);
    }

    // month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday_last operator /(int m,
                                                                      const etl::chrono::weekday_last& wdl) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday_last(etl::chrono::month(m), wdl);
    }

    // month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday_last operator /(const etl::chrono::weekday_last& wdl,
                                                                      const etl::chrono::month& m) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday_last(m, wdl);
    }

    // month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::month_weekday_last operator /(const etl::chrono::weekday_last& wdl,
                                                                      int m) ETL_NOEXCEPT
    {
      return etl::chrono::month_weekday_last(etl::chrono::month(m), wdl);
    }

    //*************************************************************************
    // year_month
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator /(const etl::chrono::year& y,
                                                              const etl::chrono::month& m) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(y, m);
    }

    // year_month
    inline ETL_CONSTEXPR14 etl::chrono::year_month operator /(const etl::chrono::year& y,
                                                              int m) ETL_NOEXCEPT
    {
      return etl::chrono::year_month(y, etl::chrono::month(m));
    }

    ////*************************************************************************
    // year_month_day
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day operator /(const etl::chrono::year_month& ym,
                                                                  const etl::chrono::day& d) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day(ym.year(), ym.month(), d);
    }

    // year_month_day
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day operator /(const etl::chrono::year_month& ym, 
                                                                  int d ) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day(ym.year(), ym.month(), etl::chrono::day(d));
    }

    // year_month_day
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day operator /(const etl::chrono::year& y,
                                                                  const etl::chrono::month_day& md) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day(y, md.month(), md.day());
    }

    // year_month_day
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day operator/(int y,
                                                                 const etl::chrono::month_day& md) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day(etl::chrono::year(y), md.month(), md.day());
    }

    // year_month_day
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day operator /(const etl::chrono::month_day& md,
                                                                  const etl::chrono::year& y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day(y, md.month(), md.day());
    }

    // year_month_day
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day operator /(const etl::chrono::month_day& md,
                                                                  int y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day(etl::chrono::year(y), md.month(), md.day());
    }

    //*************************************************************************
    // year_month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator /(const etl::chrono::year_month& ym,
                                                                       etl::chrono::last_spec) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day_last(ym.year(), etl::chrono::month_day_last(ym.month()));
    }

    // year_month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator /(const etl::chrono::year& y,
                                                                       const etl::chrono::month_day_last& mdl) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day_last(y, mdl);
    }

    // year_month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator /(int y,
                                                                       const etl::chrono::month_day_last& mdl) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day_last(etl::chrono::year(y), mdl);
    }

    // year_month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator /(const etl::chrono::month_day_last& mdl,
                                                                       const etl::chrono::year& y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day_last(y, mdl);
    }

    // year_month_day_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_day_last operator /(const etl::chrono::month_day_last& mdl,
                                                                       int y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_day_last(etl::chrono::year(y), mdl);
    }

    //*************************************************************************
    // year_month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator /(const etl::chrono::year_month& ym,
                                                                      const etl::chrono::weekday_indexed& wdi) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(ym.year(), ym.month(), wdi);
    }

    // year_month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator /(const etl::chrono::year& y,
                                                                      const etl::chrono::month_weekday& mwd) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(y, mwd.month(), mwd.weekday_indexed());
    }

    // year_month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator/(int y, 
                                                                     const etl::chrono::month_weekday& mwd) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(etl::chrono::year(y), mwd.month(), mwd.weekday_indexed());
    }

    // year_month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator /(const etl::chrono::month_weekday& mwd,
                                                                      const etl::chrono::year& y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(y, mwd.month(), mwd.weekday_indexed());
    }

    // year_month_weekday
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday operator /(const etl::chrono::month_weekday& mwd, 
                                                                      int y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday(etl::chrono::year(y), mwd.month(), mwd.weekday_indexed());
    }

    //*************************************************************************
    // year_month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator /(const etl::chrono::year_month& ym,
                                                                           const etl::chrono::weekday_last& wdl) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(ym.year(), ym.month(), wdl);
    }

    // year_month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator /(const etl::chrono::year& y,
                                                                           const etl::chrono::month_weekday_last& mwdl) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(y, mwdl.month(), mwdl.weekday_last());
    }

    // year_month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator/(int y,
                                                                          const etl::chrono::month_weekday_last& mwdl) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(etl::chrono::year(y), mwdl.month(), mwdl.weekday_last());
    }

    // year_month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator /(const etl::chrono::month_weekday_last& mwdl,
                                                                           const etl::chrono::year& y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(y, mwdl.month(), mwdl.weekday_last());
    }

    // year_month_weekday_last
    inline ETL_CONSTEXPR14 etl::chrono::year_month_weekday_last operator /(const etl::chrono::month_weekday_last& mwdl,
                                                                           int y) ETL_NOEXCEPT
    {
      return etl::chrono::year_month_weekday_last(etl::chrono::year(y), mwdl.month(), mwdl.weekday_last());
    }
  }
}