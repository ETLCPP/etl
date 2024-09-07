///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2023 John Wellbelove

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

#include "../../ratio.h"
#include "../../static_assert.h"
#include "../../limits.h"

namespace etl
{
  namespace chrono
  {
    //***********************************************************************
    /// duration
    //***********************************************************************
    template <typename TValue, typename TPeriod = etl::ratio<1> >
    class duration
    {
    public:

      //***********************************************************************
      ETL_CONSTEXPR duration() ETL_NOEXCEPT
        : value(0)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR duration(const etl::chrono::duration<TValue, TPeriod>& other) ETL_NOEXCEPT
        : value(other.value)
      {
      }

      //***********************************************************************
      template< typename TValue2 >
      ETL_CONSTEXPR explicit duration(const TValue2& value_) ETL_NOEXCEPT
        : value(static_cast<TValue>(value_))
      {
      }

      //***********************************************************************
      template< typename TValue2, typename TPeriod2 >
      ETL_CONSTEXPR duration(const etl::chrono::duration<TValue2, TPeriod2>& other) ETL_NOEXCEPT
        : value(static_cast<TValue>(other.value))
      {
        ETL_STATIC_ASSERT(!(etl::is_integral<TValue>::value && etl::is_floating_point<TValue2>::value), "Cannot convert duration from floating point to integral");
      }

      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::duration< TValue, TPeriod> zero()  ETL_NOEXCEPT
      {
        return etl::chrono::duration{ 0, TPeriod()};
      }

      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::duration<TValue, TPeriod> min()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TValue, TPeriod> { etl::numeric_limits<TValue>::min() };
      }

      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::duration<TValue, TPeriod> max()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TValue, TPeriod>{ etl::numeric_limits<TValue>::max() };
      }

      //***********************************************************************
      ETL_CONSTEXPR TValue count() const ETL_NOEXCEPT
      {
        return value;
      }

    private:

      TValue  value;
    };

    //***********************************************************************
    /// Duration types
    //***********************************************************************
#if (ETL_USING_64BIT_TYPES)
    typedef etl::chrono::duration<int64_t, etl::nano>              nanoseconds;
    typedef etl::chrono::duration<int64_t, etl::micro>             microseconds;
    typedef etl::chrono::duration<int64_t, etl::milli>             milliseconds;
    typedef etl::chrono::duration<int64_t, etl::ratio<1U> >        seconds;
#else
    typedef etl::chrono::duration<int32_t, etl::nano>              nanoseconds;
    typedef etl::chrono::duration<int32_t, etl::micro>             microseconds;
    typedef etl::chrono::duration<int32_t, etl::milli>             milliseconds;
    typedef etl::chrono::duration<int32_t, etl::ratio<1U> >        seconds;
#endif
    typedef etl::chrono::duration<int32_t, etl::ratio<60U> >       minutes;
    typedef etl::chrono::duration<int32_t, etl::ratio<3600U> >     hours;
    typedef etl::chrono::duration<int32_t, etl::ratio<86400U> >    days;
    typedef etl::chrono::duration<int32_t, etl::ratio<604800U> >   weeks;
    typedef etl::chrono::duration<int32_t, etl::ratio<2629746U> >  months;
    typedef etl::chrono::duration<int32_t, etl::ratio<31556952U> > years;

    //***********************************************************************
    /// duration_cast
    //***********************************************************************
    template <typename TToDuration, typename TValue, typename TPeriod>
    ETL_CONSTEXPR TToDuration duration_cast(const etl::chrono::duration<TValue, TPeriod>& d) ETL_NOEXCEPT
    {
      using to_value_type = typename TToDuration::value_type;
      using to_period     = typename TToDuration::period;

      // Calculate the conversion factor between the periods
      ETL_CONSTEXPR auto conversion_factor = (static_cast<double>(TPeriod::num) / TPeriod::den) * (to_period::den / to_period::num);

      // Convert the value
      to_value_type converted_value = static_cast<to_value_type>(d.count() * conversion_factor);

      return TToDuration(converted_value);
    }
  }
}
