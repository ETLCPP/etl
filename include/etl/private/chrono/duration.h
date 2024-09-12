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
    namespace private_chrono
    {
      // Helper to find the greatest common divisor
      template <intmax_t A, intmax_t B>
      struct gcd 
      {
        static ETL_CONSTANT intmax_t value = gcd<B, A % B>::value;
      };

      template <intmax_t A>
      struct gcd<A, 0> 
      {
        static ETL_CONSTANT intmax_t value = A;
      };

      // Helper to find the least common multiple
      template <intmax_t A, intmax_t B>
      struct lcm 
      {
        static ETL_CONSTANT intmax_t value = (A / gcd<A, B>::value) * B;
      };
    }

    //***********************************************************************
    /// duration_values
    //***********************************************************************
    template <typename TRep>
    struct duration_values
    {
      //***********************************************************************
      static ETL_CONSTEXPR TRep zero()
      {
        return TRep(0);
      }

      //***********************************************************************
      static ETL_CONSTEXPR TRep min()
      {
        return etl::numeric_limits<TRep>::min();
      }

      //***********************************************************************
      static ETL_CONSTEXPR TRep max()
      {
        return etl::numeric_limits<TRep>::max();
      }
    };

    //***********************************************************************
    /// duration
    //***********************************************************************
    template <typename TRep, typename TPeriod = etl::ratio<1> >
    class duration
    {
    public:

      typedef TRep    rep;
      typedef TPeriod period;

      //***********************************************************************
      ETL_CONSTEXPR duration() ETL_NOEXCEPT
        : value(0)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR duration(const etl::chrono::duration<TRep, TPeriod>& other) ETL_NOEXCEPT
        : value(other.value)
      {
      }

      //***********************************************************************
      template <typename TRep2>
      ETL_CONSTEXPR explicit duration(const TRep2& value_) ETL_NOEXCEPT
        : value(static_cast<TRep>(value_))
      {
      }

      //***********************************************************************
      template <typename TRep2, typename TPeriod2>
      ETL_CONSTEXPR duration(const etl::chrono::duration<TRep2, TPeriod2>& other) ETL_NOEXCEPT
        : value(static_cast<TRep>(other.count()))
      {
        ETL_STATIC_ASSERT(!(etl::is_integral<TRep>::value && etl::is_floating_point<TRep2>::value), "Cannot convert duration from floating point to integral");
      }

      //***********************************************************************
      ETL_CONSTEXPR TRep count() const ETL_NOEXCEPT
      {
        return value;
      }

      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::duration<TRep, TPeriod> zero()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TRep, TPeriod>(etl::chrono::duration_values<TRep>::zero());
      }

      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::duration<TRep, TPeriod> min()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TRep, TPeriod>(etl::chrono::duration_values<TRep>::min());
      }

      //***********************************************************************
      static ETL_CONSTEXPR etl::chrono::duration<TRep, TPeriod> max()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TRep, TPeriod>(etl::chrono::duration_values<TRep>::max());
      }

    private:

      TRep  value;
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
    template <typename TToDuration, typename TRep, typename TPeriod>
    ETL_CONSTEXPR TToDuration duration_cast(const etl::chrono::duration<TRep, TPeriod>& d) ETL_NOEXCEPT
    {
      typedef TRep    from_rep;
      typedef TPeriod from_period;

      typedef typename TToDuration::rep    to_rep;
      typedef typename TToDuration::period to_period;

      typedef typename etl::ratio_divide<from_period, to_period>::type ratio_divide_t;
      typedef typename etl::common_type<from_rep, to_rep, intmax_t>::type common_t;

      common_t ct_count = static_cast<common_t>(d.count());
      common_t ct_num   = static_cast<common_t>(ratio_divide_t::type::num);
      common_t ct_den   = static_cast<common_t>(ratio_divide_t::type::den);

      if ETL_IF_CONSTEXPR((from_period::num == to_period::num) && (from_period::den == to_period::den))
      {
        return TToDuration(static_cast<to_rep>(d.count()));
      }
      else if ETL_IF_CONSTEXPR(ratio_divide_t::num == 1)
      {
        return TToDuration(static_cast<to_rep>(ct_count / ct_den));
      }
      else if ETL_IF_CONSTEXPR(ratio_divide_t::den == 1)
      {
        return TToDuration(static_cast<to_rep>(ct_count * ct_num));
      }
      else
      {
        return TToDuration(static_cast<to_rep>((ct_count * ct_num) / ct_den));
      }
    }
  }

  //*************************************************************************
  /// Hash function for etl::chrono::duration
  //*************************************************************************
#if ETL_USING_8BIT_TYPES
  template <typename TRep, typename TPeriod>
  struct hash<etl::chrono::duration<TRep, TPeriod> >
  {
    size_t operator()(const etl::chrono::duration<TRep, TPeriod>& d) const
    {
      TRep value = d.count();
      size_t num = TPeriod::num;
      size_t den = TPeriod::den;

      return 0; //etl::private_hash::generic_hash<size_t>(p, p + sizeof(unsigned));
    }
  };
#endif

  //***********************************************************************
  /// Find the common type of two duration types.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  struct common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2> >
  {
  private:

    typedef typename etl::common_type<TRep1, TRep2>::type value_type;
    typedef etl::ratio<etl::chrono::private_chrono::gcd<TPeriod1::num, TPeriod2::num>::value,
                       etl::chrono::private_chrono::lcm<TPeriod1::den, TPeriod2::den>::value> period_type;

  public:

    using type = etl::chrono::duration<value_type, period_type>;
  };
}

#if ETL_HAS_CHRONO_LITERALS_DAY
#if ETL_USING_CPP11
namespace etl
{
  namespace literals
  {
    namespace chrono_literals
    {
      //***********************************************************************
      /// Literal for hours duration
      //***********************************************************************
      ETL_IF_CONSTEXPR etl::chrono::hours operator ""_hours(unsigned long long h) noexcept
      {
        return etl::chrono::hours(static_cast<etl::chrono::hours::rep>(h));
      }

      //***********************************************************************
      /// Literal for minutes duration
      //***********************************************************************
      ETL_IF_CONSTEXPR etl::chrono::hours operator ""_minutes(unsigned long long m) noexcept
      {
        return etl::chrono::minutes(static_cast<etl::chrono::minutes::rep>(m));
      }

      //***********************************************************************
      /// Literal for seconds duration
      //***********************************************************************
      ETL_IF_CONSTEXPR etl::chrono::seconds operator ""_seconds(unsigned long long s) noexcept
      {
        return etl::chrono::seconds(static_cast<etl::chrono::seconds::rep>(s));
      }

      //***********************************************************************
      /// Literal for milliseconds duration
      //***********************************************************************
      ETL_IF_CONSTEXPR etl::chrono::milliseconds operator ""_milliseconds(unsigned long long s) noexcept
      {
        return etl::chrono::milliseconds(static_cast<etl::chrono::milliseconds::rep>(s));
      }

      //***********************************************************************
      /// Literal for microseconds duration
      //***********************************************************************
      ETL_IF_CONSTEXPR etl::chrono::microseconds operator ""_microseconds(unsigned long long s) noexcept
      {
        return etl::chrono::microseconds(static_cast<etl::chrono::microseconds::rep>(s));
      }

      //***********************************************************************
      /// Literal for nanoseconds duration
      //***********************************************************************
      ETL_IF_CONSTEXPR etl::chrono::nanoseconds operator ""_nanoseconds(unsigned long long s) noexcept
      {
        return etl::chrono::nanoseconds(static_cast<etl::chrono::nanoseconds::rep>(s));
      }
    }
  }
}
#endif
#endif
