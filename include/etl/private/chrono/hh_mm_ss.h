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

#include "../../absolute.h"
#include "../../power.h"

namespace etl
{
  //***********************************************************************
  /// absolute
  /// Enabled for etl::chrono::duration
  //***********************************************************************
  template <typename TDuration>
  ETL_NODISCARD
  ETL_CONSTEXPR 
  typename etl::enable_if<etl::chrono::is_duration<TDuration>::value, TDuration>::type
    absolute(TDuration dur) ETL_NOEXCEPT
  {
    return TDuration((dur.count() < 0) ? -dur.count() : dur.count());
  }

  namespace chrono
  {
    //***********************************************************************
    /// hh_mm_ss
    //***********************************************************************
    template <typename TDuration>
    class hh_mm_ss
    {
    public:

      ETL_STATIC_ASSERT(etl::chrono::is_duration<TDuration>::value, "TDuration not etl::chrono::duration");

      //***********************************************************************
      // Fractional width for the duration's subseconds
      //***********************************************************************
      static constexpr int fractional_width = []() constexpr 
        {
          if (TDuration::period::den == 1) 
          {
            return 0;
          }

          int width = 0;

          for (auto den = TDuration::period::den; den > 1; den /= 10) 
          {
            ++width;
          }

          return width;
        }();

      //***********************************************************************
      /// The return type for to_duration.
      //***********************************************************************
      using precision = etl::chrono::duration<common_type_t<typename TDuration::rep, etl::chrono::seconds::rep>, 
                                              ratio<1, etl::power<10, fractional_width>::value>>;

      //***********************************************************************
      /// Default constructor.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR 
      hh_mm_ss() ETL_NOEXCEPT
        : d(TDuration::zero())
      {
      }

      //***********************************************************************
      /// Construct from duration.
      //***********************************************************************
      ETL_NODISCARD 
      ETL_CONSTEXPR
      explicit hh_mm_ss(TDuration d_) ETL_NOEXCEPT
        : d(d_)
      {
      }

      //***********************************************************************
      /// Checks for negative duration.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR 
      bool is_negative() const ETL_NOEXCEPT
      {
        return d < TDuration::zero();
      }

      //***********************************************************************
      /// Returns the hours.
      //***********************************************************************
      ETL_NODISCARD 
      ETL_CONSTEXPR 
      etl::chrono::hours hours() const ETL_NOEXCEPT
      {
        auto dur = etl::absolute(d);

        return etl::chrono::duration_cast<etl::chrono::hours>(dur);
      }

      //***********************************************************************
      /// Returns the minutes.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR etl::chrono::minutes minutes() const ETL_NOEXCEPT
      {
        auto dur = etl::absolute(d) - hours();

        return etl::chrono::duration_cast<etl::chrono::minutes>(dur);
      }

      //***********************************************************************
      /// Returns the seconds.
      //***********************************************************************
      ETL_NODISCARD 
      ETL_CONSTEXPR
      etl::chrono::seconds seconds() const ETL_NOEXCEPT
      {
        auto dur = etl::absolute(d) - hours() - minutes();

        return etl::chrono::duration_cast<etl::chrono::seconds>(dur);
      }

      //***********************************************************************
      /// Returns the subseconds.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR precision subseconds() const ETL_NOEXCEPT
      {
        return etl::absolute(d) - etl::chrono::duration_cast<etl::chrono::seconds>(etl::absolute(d));
      }

      //***********************************************************************
      /// Returns the duration.
      //***********************************************************************
      ETL_CONSTEXPR explicit operator precision() const ETL_NOEXCEPT
      {
        return to_duration();
      }

      //***********************************************************************
      /// Returns the duration.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR 
      precision to_duration() const ETL_NOEXCEPT
      {
        return d;
      }

    private:
      
      TDuration d;
    };
  }
}
