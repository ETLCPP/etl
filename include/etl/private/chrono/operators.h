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
    //// year_month
    //constexpr auto operator/(const etl::chrono::year& y,
    //                         const etl::chrono::month& m ) noexcept
    //  -> etl::chrono::year_month;

    //constexpr auto operator/( const etl::chrono::year& y, int m ) noexcept
    //  -> etl::chrono::year_month;

    //// month_day
    //constexpr auto operator/( const etl::chrono::month& m,
    //                           const etl::chrono::day& d ) noexcept
    //  -> etl::chrono::month_day;

    //constexpr auto operator/( const etl::chrono::month& m, int d ) noexcept
    //  -> etl::chrono::month_day;

    //constexpr auto operator/( int m, const etl::chrono::day& d ) noexcept
    //  -> etl::chrono::month_day;

    //constexpr auto operator/( const etl::chrono::day& d,
    //                           const etl::chrono::month& m ) noexcept
    //  -> etl::chrono::month_day;

    //constexpr auto operator/( const etl::chrono::day& d, int m ) noexcept
    //  -> etl::chrono::month_day;

    //// month_day_last
    //constexpr auto operator/( const etl::chrono::month& m,
    //                           etl::chrono::last_spec ) noexcept
    //  -> etl::chrono::month_day_last;

    //constexpr auto operator/( int m, etl::chrono::last_spec ) noexcept
    //  -> etl::chrono::month_day_last;

    //constexpr auto operator/( etl::chrono::last_spec,
    //                           const etl::chrono::month& m ) noexcept
    //  -> etl::chrono::month_day_last;

    //constexpr auto operator/( etl::chrono::last_spec, int m ) noexcept
    //  -> etl::chrono::month_day_last;

    //// month_weekday
    //constexpr auto operator/( const etl::chrono::month& m,
    //                           const etl::chrono::weekday_indexed& wdi ) noexcept
    //  -> etl::chrono::month_weekday;

    //constexpr auto operator/( int m, const etl::chrono::weekday_indexed& wdi ) noexcept
    //  -> etl::chrono::month_weekday;

    //constexpr auto operator/(const etl::chrono::weekday_indexed& wdi,
    //                         const etl::chrono::month& m ) noexcept
    //  -> etl::chrono::month_weekday;

    //constexpr auto operator/( const etl::chrono::weekday_indexed& wdi, int m ) noexcept
    //  -> etl::chrono::month_weekday;

    //// month_weekday_last
    //constexpr auto operator/( const etl::chrono::month& m,
    //                           const etl::chrono::weekday_last& wdl ) noexcept
    //  -> etl::chrono::month_weekday_last;

    //constexpr auto operator/( int m, const etl::chrono::weekday_last& wdl ) noexcept
    //  -> etl::chrono::month_weekday_last;

    //constexpr auto operator/( const etl::chrono::weekday_last& wdl,
    //                           const etl::chrono::month& m ) noexcept
    //  -> etl::chrono::month_weekday_last;

    //constexpr auto operator/( const etl::chrono::weekday_last& wdl, int m ) noexcept
    //  -> etl::chrono::month_weekday_last;

    //// year_month_day
    //constexpr auto operator/( const etl::chrono::year_month& ym,
    //                           const etl::chrono::day& d ) noexcept
    //  -> etl::chrono::year_month_day;

    //constexpr auto operator/( const etl::chrono::year_month& ym, int d ) noexcept
    //  -> etl::chrono::year_month_day;

    //constexpr auto operator/( const etl::chrono::year& y,
    //                           const etl::chrono::month_day& md ) noexcept
    //  -> etl::chrono::year_month_day;

    //constexpr auto operator/( int y, const etl::chrono::month_day& md ) noexcept
    //  -> etl::chrono::year_month_day;

    //constexpr auto operator/( const etl::chrono::month_day& md,
    //                           const etl::chrono::year& y ) noexcept
    //  -> etl::chrono::year_month_day;

    //constexpr auto operator/( const etl::chrono::month_day& md, int y ) noexcept
    //  -> etl::chrono::year_month_day;

    //// year_month_day_last
    //constexpr auto operator/( const etl::chrono::year_month& ym,
    //                           etl::chrono::last_spec ) noexcept
    //  -> etl::chrono::year_month_day_last;

    //constexpr auto operator/( const etl::chrono::year& y,
    //                           const etl::chrono::month_day_last& mdl ) noexcept
    //  -> etl::chrono::year_month_day_last;

    //constexpr auto operator/( int y, const etl::chrono::month_day_last& mdl ) noexcept
    //  -> etl::chrono::year_month_day_last;

    //constexpr auto operator/( const etl::chrono::month_day_last& mdl,
    //                           const etl::chrono::year& y ) noexcept
    //  -> etl::chrono::year_month_day_last;

    //constexpr auto operator/( const etl::chrono::month_day_last& mdl, int y ) noexcept
    //  -> etl::chrono::year_month_day_last;

    //// year_month_weekday
    //constexpr auto operator/( const etl::chrono::year_month& ym,
    //                           const etl::chrono::weekday_indexed& wdi ) noexcept
    //  -> etl::chrono::year_month_weekday;

    //constexpr auto operator/( const etl::chrono::year& y,
    //                           const etl::chrono::month_weekday& mwd ) noexcept
    //  -> etl::chrono::year_month_weekday;

    //constexpr auto operator/( int y, const etl::chrono::month_weekday& mwd ) noexcept
    //  -> etl::chrono::year_month_weekday;

    //constexpr auto operator/( const etl::chrono::month_weekday& mwd,
    //                           const etl::chrono::year& y ) noexcept
    //  -> etl::chrono::year_month_weekday;

    //constexpr auto operator/( const etl::chrono::month_weekday& mwd, int y ) noexcept
    //  -> etl::chrono::year_month_weekday;

    //// year_month_weekday_last
    //constexpr auto operator/( const etl::chrono::year_month& ym,
    //                           const etl::chrono::weekday_last& wdl ) noexcept
    //  -> etl::chrono::year_month_weekday_last;

    //constexpr auto operator/( const etl::chrono::year& y,
    //                           const etl::chrono::month_weekday_last& mwdl ) noexcept
    //  -> etl::chrono::year_month_weekday_last;

    //constexpr auto operator/( int y, const etl::chrono::month_weekday_last& mwdl ) noexcept
    //  -> etl::chrono::year_month_weekday_last;

    //constexpr auto operator/( const etl::chrono::month_weekday_last& mwdl,
    //                           const etl::chrono::year& y ) noexcept
    //  -> etl::chrono::year_month_weekday_last;

    //constexpr auto operator/( const etl::chrono::month_weekday_last& mwdl, int y ) noexcept
    //  -> etl::chrono::year_month_weekday_last;
  }
}