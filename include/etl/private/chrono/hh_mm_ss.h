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
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_specialization<TDuration, etl::chrono::duration>::value, TDuration>::type
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
    private:

      // Helper template to calculate the number of digits in a denominator at compile time
      template <uintmax_t Den, int Width = 0>
      struct fractional_width_helper
      {
        static constexpr int value = fractional_width_helper<Den / 10, Width + 1>::value;
      };

      // Base case: when Den == 1, stop recursion
      template <int Width>
      struct fractional_width_helper<1, Width>
      {
        static constexpr int value = Width;
      };

      // Special case: when Den == 0 (invalid denominator), return 0
      template <int Width>
      struct fractional_width_helper<0, Width>
      {
        static constexpr int value = 0;
      };

      // Calculate fractional width for TDuration
      template <typename TDur>
      struct calculate_fractional_width
      {
        static constexpr int value = (TDur::period::den == 1)
                                     ? 0
                                     : fractional_width_helper<TDur::period::den>::value;
      };

    public:

      ETL_STATIC_ASSERT((etl::is_specialization<TDuration, etl::chrono::duration>::value), "TDuration is not a etl::chrono::duration type");

      static constexpr int fractional_width = calculate_fractional_width<TDuration>::value;

      //***********************************************************************
      /// The return type for to_duration.
      //***********************************************************************
      using precision = etl::chrono::duration<common_type_t<typename TDuration::rep, etl::chrono::seconds::rep>, 
                                              ratio<1, etl::power<10, fractional_width>::value>>;

      //***********************************************************************
      /// Default constructor.
      //***********************************************************************
      ETL_CONSTEXPR 
      hh_mm_ss() ETL_NOEXCEPT
        : d(TDuration::zero())
      {
      }

      //***********************************************************************
      /// Construct from duration.
      //***********************************************************************
      ETL_CONSTEXPR14
      explicit hh_mm_ss(TDuration d_) ETL_NOEXCEPT
        : d(d_)
      {
      }

      //***********************************************************************
      /// Checks for negative duration.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 
      bool is_negative() const ETL_NOEXCEPT
      {
        return d < TDuration::zero();
      }

      //***********************************************************************
      /// Returns the hours.
      //***********************************************************************
      ETL_NODISCARD 
      ETL_CONSTEXPR14 
      etl::chrono::hours hours() const ETL_NOEXCEPT
      {
        auto dur = etl::absolute(d);

        return etl::chrono::duration_cast<etl::chrono::hours>(dur);
      }

      //***********************************************************************
      /// Returns the minutes.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::minutes minutes() const ETL_NOEXCEPT
      {
        auto dur = etl::absolute(d) - hours();

        return etl::chrono::duration_cast<etl::chrono::minutes>(dur);
      }

      //***********************************************************************
      /// Returns the seconds.
      //***********************************************************************
      ETL_NODISCARD 
      ETL_CONSTEXPR14
      etl::chrono::seconds seconds() const ETL_NOEXCEPT
      {
        auto dur = etl::absolute(d) - hours() - minutes();

        return etl::chrono::duration_cast<etl::chrono::seconds>(dur);
      }

      //***********************************************************************
      /// Returns the subseconds.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 precision subseconds() const ETL_NOEXCEPT
      {
        return etl::absolute(d) - etl::chrono::duration_cast<etl::chrono::seconds>(etl::absolute(d));
      }

      //***********************************************************************
      /// Returns the duration.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 explicit operator precision() const ETL_NOEXCEPT
      {
        return to_duration();
      }

      //***********************************************************************
      /// Returns the duration.
      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 
      precision to_duration() const ETL_NOEXCEPT
      {
        return d;
      }

    private:
      
      TDuration d;
    };

    template <typename TDuration>
    constexpr int etl::chrono::hh_mm_ss<TDuration>::fractional_width;
  }
}
