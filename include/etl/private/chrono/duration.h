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
FITNESS FOR Value1 PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
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
#include "../../type_traits.h"

#include <string.h>

namespace etl
{
  namespace chrono
  {
    namespace private_chrono
    {
      // Helper to find the greatest common divisor
      template <intmax_t Value1, intmax_t Value2>
      struct gcd 
      {
        static ETL_CONSTANT intmax_t value = gcd<Value2, Value1 % Value2>::value;
      };

      template <intmax_t Value1>
      struct gcd<Value1, 0> 
      {
        static ETL_CONSTANT intmax_t value = Value1;
      };

      // Helper to find the least common multiple
      template <intmax_t Value1, intmax_t Value2>
      struct lcm 
      {
        static ETL_CONSTANT intmax_t value = (Value1 / gcd<Value1, Value2>::value) * Value2;
      };
    }

    //***********************************************************************
    /// duration_values
    //***********************************************************************
    template <typename TRep>
    struct duration_values
    {
      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR TRep zero() ETL_NOEXCEPT
      {
        return TRep(0);
      }

      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 TRep min() ETL_NOEXCEPT
      {
        return etl::numeric_limits<TRep>::min();
      }

      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 TRep max() ETL_NOEXCEPT
      {
        return etl::numeric_limits<TRep>::max();
      }
    };

    template <typename TRep, typename TPeriod>
    class duration;

    template <typename TToDuration, typename TRep, typename TPeriod>
    ETL_CONSTEXPR14 TToDuration duration_cast(const etl::chrono::duration<TRep, TPeriod>& d) ETL_NOEXCEPT;

    //***********************************************************************
    /// duration
    //***********************************************************************
    template <typename TRep, typename TPeriod = etl::ratio<1> >
    class duration
    {
    public:

      using rep    = TRep;
      using period =  typename TPeriod::type;

      //***********************************************************************
      ETL_CONSTEXPR duration() ETL_NOEXCEPT
        : value(0)
      {
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration(const etl::chrono::duration<TRep, TPeriod>& other) ETL_NOEXCEPT
        : value(other.value)
      {
      }

      //***********************************************************************
      template <typename TRep2>
      ETL_CONSTEXPR14 explicit duration(const TRep2& value_) ETL_NOEXCEPT
        : value(static_cast<TRep>(value_))
      {
      }

      //***********************************************************************
      template <typename TRep2, typename TPeriod2, typename etl::enable_if<etl::ratio_divide<TPeriod2, TPeriod>::den == 1, int>::type = 0>
      ETL_CONSTEXPR14 duration(const etl::chrono::duration<TRep2, TPeriod2>& other) ETL_NOEXCEPT
        : value(etl::chrono::duration_cast<etl::chrono::duration<TRep, TPeriod> >(other).count())
      {
        ETL_STATIC_ASSERT(!(etl::is_integral<TRep>::value && etl::is_floating_point<TRep2>::value), "Cannot convert duration from floating point to integral");
      }

      //***********************************************************************
      ETL_CONSTEXPR14 
      etl::chrono::duration<TRep, TPeriod> operator =(const etl::chrono::duration<TRep, TPeriod>& other) ETL_NOEXCEPT
      {
        value = other.count();

        return *this;
      }

      //***********************************************************************
      template <typename TRep2, typename TPeriod2>
      ETL_CONSTEXPR14 
      etl::chrono::duration<TRep, TPeriod> operator =(const etl::chrono::duration<TRep2, TPeriod2>& other) ETL_NOEXCEPT
      {
        value = etl::chrono::duration_cast<etl::chrono::duration<TRep, TPeriod> >(other).count();

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 TRep count() const ETL_NOEXCEPT
      {
        return value;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 etl::common_type_t<duration> operator +() const ETL_NOEXCEPT
      {
        return etl::common_type_t<duration>(*this);
      }

      //***********************************************************************
      ETL_CONSTEXPR14 etl::common_type_t<duration> operator -() const ETL_NOEXCEPT
      {
        return etl::common_type_t<duration>(-value);
      }

      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> zero()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TRep, TPeriod>(etl::chrono::duration_values<TRep>::zero());
      }

      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> min()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TRep, TPeriod>(etl::chrono::duration_values<TRep>::min());
      }

      //***********************************************************************
      ETL_NODISCARD
      static ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> max()  ETL_NOEXCEPT
      {
        return etl::chrono::duration<TRep, TPeriod>(etl::chrono::duration_values<TRep>::max());
      }

      //***********************************************************************
      ETL_NODISCARD
      ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> absolute() const ETL_NOEXCEPT
      {
        return etl::chrono::duration<TRep, TPeriod>(value < 0 ? -value : value);
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator ++() ETL_NOEXCEPT
      {
        ++value;

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration operator ++(int) ETL_NOEXCEPT
      {
        duration temp(*this);      
        ++value;

        return temp;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator --() ETL_NOEXCEPT
      {
        --value;

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration operator --(int) ETL_NOEXCEPT
      {
        duration temp(*this);
        --value;

        return temp;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator +=(const duration<TRep, TPeriod>& d) ETL_NOEXCEPT
      {
        value += d.count();

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator -=(const duration<TRep, TPeriod>& d) ETL_NOEXCEPT
      {
        value -= d.count();

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator *=(const TRep& r) ETL_NOEXCEPT
      {
        value *= r;

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator /=(const TRep& r) ETL_NOEXCEPT
      {
        value /= r;

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator %=(const TRep& r) ETL_NOEXCEPT
      {
        value %= r;

        return *this;
      }

      //***********************************************************************
      ETL_CONSTEXPR14 duration& operator %=(const duration<TRep, TPeriod>& d) ETL_NOEXCEPT
      {
        value %= d.count();

        return *this;
      }

      //***********************************************************************
      /// Compare duration with another.
      /// if duration < other, returns -1
      /// else if duration > other, returns 1
      /// else returns 0
      //***********************************************************************
      template <typename TRep2, typename TPeriod2>
      ETL_CONSTEXPR14 int compare(const duration<TRep2, TPeriod2>& other) const ETL_NOEXCEPT 
      {
        // Determine the common type of the two durations.
        using common_duration = typename etl::common_type<etl::chrono::duration<TRep, TPeriod>, etl::chrono::duration<TRep2, TPeriod2>>::type;

        common_duration lhs_converted = etl::chrono::duration_cast<common_duration>(*this);
        common_duration rhs_converted = etl::chrono::duration_cast<common_duration>(other);

        if (lhs_converted.count() < rhs_converted.count()) return -1;
        if (lhs_converted.count() > rhs_converted.count()) return 1;

        return 0;
      }

    private:

      TRep value;
    };

    //***********************************************************************
    /// Duration types
    //***********************************************************************
#if (ETL_USING_64BIT_TYPES)
    using nanoseconds  = etl::chrono::duration<int64_t, etl::nano>;
    using microseconds = etl::chrono::duration<int64_t, etl::micro>;
    using milliseconds = etl::chrono::duration<int64_t, etl::milli>;
    using seconds      = etl::chrono::duration<int64_t, etl::ratio<1U>>;
#else
    using nanoseconds  = etl::chrono::duration<int32_t, etl::nano>;
    using microseconds = etl::chrono::duration<int32_t, etl::micro>;
    using milliseconds = etl::chrono::duration<int32_t, etl::milli>;
    using seconds      = etl::chrono::duration<int32_t, etl::ratio<1U>>;
#endif
    using minutes = etl::chrono::duration<int32_t, etl::ratio<60U>>;
    using hours   = etl::chrono::duration<int32_t, etl::ratio<3600U>>;
    using days    = etl::chrono::duration<int32_t, etl::ratio<86400U>>;
    using weeks   = etl::chrono::duration<int32_t, etl::ratio<604800U>>;
    using months  = etl::chrono::duration<int32_t, etl::ratio<2629746U>>;
    using years   = etl::chrono::duration<int32_t, etl::ratio<31556952U>>;

    //***********************************************************************
    /// duration_cast
    //***********************************************************************
    template <typename TToDuration, typename TRep, typename TPeriod>
    ETL_CONSTEXPR14 TToDuration duration_cast(const etl::chrono::duration<TRep, TPeriod>& d) ETL_NOEXCEPT
    {
      using from_rep    = TRep    ;
      using from_period = TPeriod ;

      using to_rep    = typename TToDuration::rep;
      using to_period = typename TToDuration::period;

      using ratio_divide_t = typename etl::ratio_divide<from_period, to_period>;
      using common_t       = typename etl::common_type<from_rep, to_rep, intmax_t>::type;

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
    ETL_CONSTEXPR14 size_t operator()(const etl::chrono::duration<TRep, TPeriod>& d) const ETL_NOEXCEPT
    {
      uint8_t buffer[sizeof(TRep) + sizeof(intmax_t) + sizeof(intmax_t)];

      TRep value = d.count();
      intmax_t num = TPeriod::num;
      intmax_t den = TPeriod::den;
      
      memcpy(buffer, &value, sizeof(TRep));
      memcpy(buffer + sizeof(TRep), &num, sizeof(intmax_t));
      memcpy(buffer + sizeof(TRep) + sizeof(intmax_t), &den, sizeof(intmax_t));

      return etl::private_hash::generic_hash<size_t>(buffer, buffer + sizeof(TRep) + sizeof(intmax_t) + sizeof(intmax_t));
    }
  };
#endif

  //***********************************************************************
  /// Find the common type of two duration types.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  struct common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2>>
  {
  private:

    using value_type  = typename etl::common_type<TRep1, TRep2>::type ;
    using period_type = etl::ratio<etl::chrono::private_chrono::gcd<TPeriod1::num, TPeriod2::num>::value,
                                   etl::chrono::private_chrono::lcm<TPeriod1::den, TPeriod2::den>::value>;

  public:

    using type = etl::chrono::duration<value_type, period_type>;
  };

  //***********************************************************************
  /// Check equality.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 bool operator ==(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                                   const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    using common_t = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2> >::type;
    
    common_t l = etl::chrono::duration_cast<common_t>(lhs);
    common_t r = etl::chrono::duration_cast<common_t>(rhs);

    return l.count() == r.count();
  }

  //***********************************************************************
  /// Check inequality.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 bool operator !=(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                                   const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    return !(lhs == rhs);
  }

  //***********************************************************************
  /// Less-than.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 bool operator <(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                                  const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    using common_t = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2> >::type;

    common_t l = etl::chrono::duration_cast<common_t>(lhs);
    common_t r = etl::chrono::duration_cast<common_t>(rhs);

    return l.count() < r.count();
  }

  //***********************************************************************
  /// Less-than-or-equal.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 bool operator <=(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                                   const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    return !(rhs < lhs);
  }

  //***********************************************************************
  /// Greater-than.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 bool operator >(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                                  const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    return rhs < lhs;
  }

  //***********************************************************************
  /// Greater-than-or-equal.
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 bool operator >=(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                                   const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    return !(lhs < rhs);
  }

  //***********************************************************************
  /// Spaceship operator
  //***********************************************************************
#if ETL_USING_CPP20
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  [[nodiscard]] constexpr auto operator <=>(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
                                            const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    using common_t = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2> >::type;

    common_t l = etl::chrono::duration_cast<common_t>(lhs);
    common_t r = etl::chrono::duration_cast<common_t>(rhs);

    return (l.count() <=> r.count());
  }
#endif

  //***********************************************************************
  /// Operator +
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2> >::type 
    operator +(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
               const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    // Determine the common type of the two durations.
    using common_duration = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2>>::type;

    // Convert both durations to the common type.
    common_duration lhs_converted = etl::chrono::duration_cast<common_duration>(lhs);
    common_duration rhs_converted = etl::chrono::duration_cast<common_duration>(rhs);

    // Return the sum of the two converted durations.
    return common_duration(lhs_converted.count() + rhs_converted.count());
  }

  //***********************************************************************
  /// Operator -
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2> >::type 
    operator -(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
               const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    // Determine the common type of the two durations.
    using common_duration = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2>>::type;

    // Convert both durations to the common type.
    common_duration lhs_converted = etl::chrono::duration_cast<common_duration>(lhs);
    common_duration rhs_converted = etl::chrono::duration_cast<common_duration>(rhs);

    // Return the difference of the two converted durations.
    return common_duration(lhs_converted.count() - rhs_converted.count());
  }

  //***********************************************************************
  /// Operator *
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2>
  ETL_CONSTEXPR14 
  typename enable_if<!etl::is_specialization<TRep2, etl::chrono::duration>::value, etl::chrono::duration<typename etl::common_type<TRep1, TRep2>::type, TPeriod1>>::type
    operator *(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
               const TRep2& rhs) ETL_NOEXCEPT
  {
    using common_rep      = typename etl::common_type<TRep1, TRep2>::type;
    using result_duration = etl::chrono::duration<common_rep, TPeriod1>;

    // Multiply the count of the duration by the scalar value
    return result_duration(static_cast<common_rep>(lhs.count()) * static_cast<common_rep>(rhs));
  }

  //***********************************************************************
  /// Operator *
  //***********************************************************************
  template <typename TRep1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 etl::chrono::duration<typename etl::common_type<TRep1, TRep2>::type, TPeriod2>
    operator *(const TRep1& lhs, 
               const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    using common_rep      = typename etl::common_type<TRep1, TRep2>::type;
    using result_duration = etl::chrono::duration<common_rep, TPeriod2>;

    // Multiply the count of the duration by the scalar value
    return result_duration(static_cast<common_rep>(rhs.count()) * static_cast<common_rep>(lhs));
  }

  //***********************************************************************
  /// Operator /
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2>
  ETL_CONSTEXPR14 
  typename enable_if<!etl::is_specialization<TRep2, etl::chrono::duration>::value, etl::chrono::duration<typename etl::common_type<TRep1, TRep2>::type, TPeriod1>>::type
    operator /(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
               const TRep2& rhs) ETL_NOEXCEPT
  {
    using common_rep      = typename etl::common_type<TRep1, TRep2>::type;
    using result_duration = etl::chrono::duration<common_rep, TPeriod1>;

    // Divide the count of the duration by the scalar value
    return result_duration(static_cast<common_rep>(lhs.count()) / static_cast<common_rep>(rhs));
  }

  //***********************************************************************
  /// Operator /
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 typename etl::common_type<TRep1, TRep2>::type
    operator /(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
               const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    // Determine the common type of the two durations.
    using common_duration = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2>>::type;

    common_duration lhs_converted = etl::chrono::duration_cast<common_duration>(lhs);
    common_duration rhs_converted = etl::chrono::duration_cast<common_duration>(rhs);

    return typename etl::common_type<TRep1, TRep2>::type(lhs_converted.count() / rhs_converted.count());
  }

  //***********************************************************************
  /// Operator %
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2>
  ETL_CONSTEXPR14 
  etl::chrono::duration<typename etl::common_type<TRep1, TRep2>::type, TPeriod1>
    operator %(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
               const TRep2& rhs) ETL_NOEXCEPT
  {
    using common_rep = typename etl::common_type<TRep1, TRep2>::type;
    using common_dur = etl::chrono::duration<common_rep, TPeriod1>;

    // Mod the count of the duration by the scalar value
    return common_dur(static_cast<common_rep>(lhs.count()) % rhs);
  }

  //***********************************************************************
  /// Operator %
  //***********************************************************************
  template <typename TRep1, typename TPeriod1, typename TRep2, typename TPeriod2>
  ETL_CONSTEXPR14 
  typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2>>::type
    operator %(const etl::chrono::duration<TRep1, TPeriod1>& lhs, 
               const etl::chrono::duration<TRep2, TPeriod2>& rhs) ETL_NOEXCEPT
  {
    // Determine the common type of the two durations.
    using common_duration = typename etl::common_type<etl::chrono::duration<TRep1, TPeriod1>, etl::chrono::duration<TRep2, TPeriod2>>::type;

    common_duration lhs_converted = etl::chrono::duration_cast<common_duration>(lhs);
    common_duration rhs_converted = etl::chrono::duration_cast<common_duration>(rhs);

    return common_duration(lhs_converted.count() % rhs_converted.count());
  }

  //***********************************************************************
  /// Rounds down a duration to the nearest lower precision.
  //***********************************************************************
  template <typename TToDuration, typename TRep, typename TPeriod>
  ETL_CONSTEXPR14
  typename etl::enable_if<etl::is_specialization<TToDuration, etl::chrono::duration>::value, TToDuration>::type
     floor(const etl::chrono::duration<TRep, TPeriod>& d) ETL_NOEXCEPT
  {
    TToDuration result = etl::chrono::duration_cast<TToDuration>(d);

    if (result > d)
    {
      --result;
    }

    return result;
  }

  //***********************************************************************
  /// Rounds up a duration to the nearest higher precision.
  //***********************************************************************
  template <typename TToDuration, typename TRep, typename TPeriod>
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_specialization<TToDuration, etl::chrono::duration>::value, TToDuration>::type
    ceil(const etl::chrono::duration<TRep, TPeriod>& d) ETL_NOEXCEPT
  {
    TToDuration result = etl::chrono::duration_cast<TToDuration>(d);

    if (result < d)
    {
      ++result;
    }

    return result;
  }

  //***********************************************************************
  /// Rounds a duration to the nearest precision.
  /// If the duration is exactly halfway, it rounds away from zero.
  //***********************************************************************
  template <typename TToDuration, typename TRep, typename TPeriod>
  ETL_CONSTEXPR14 
  typename etl::enable_if<etl::is_specialization<TToDuration, etl::chrono::duration>::value, TToDuration>::type
    round(const etl::chrono::duration<TRep, TPeriod>& d) ETL_NOEXCEPT
  {
    // Convert the input duration to the target duration type
    TToDuration lower = floor<TToDuration>(d);
    TToDuration upper = ceil<TToDuration>(lower + TToDuration(1));

    auto lower_diff = d - lower;
    auto upper_diff = upper - d;

    if ((lower_diff < upper_diff) || 
        ((lower_diff == upper_diff) &&
         etl::is_even(lower.count()))) 
    {
      return lower;
    }
    else
    {
      return upper;
    }
  }

  //***********************************************************************
  /// Returns the absolute value of a duration.
  //***********************************************************************
  template<class TRep, class TPeriod, typename = etl::enable_if_t<etl::numeric_limits<TRep>::is_signed>>
  ETL_CONSTEXPR14 etl::chrono::duration<TRep, TPeriod> abs(etl::chrono::duration<TRep, TPeriod> d) ETL_NOEXCEPT
  {
    return d.count() >= 0 ? +d : -d;
  }
}

#if ETL_HAS_CHRONO_LITERALS_DURATION
namespace etl
{
  inline namespace literals
  {
    inline namespace chrono_literals
    {
      //***********************************************************************
      /// Literal for hours duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::hours operator ""_hours(unsigned long long h) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::hours operator ""_h(unsigned long long h) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::hours(static_cast<etl::chrono::hours::rep>(h));
      }

      //***********************************************************************
      /// Literal for floating point hours duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, ratio<3600>> operator""_hours(long double h) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, ratio<3600>> operator""_h(long double h) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::duration<double, ratio<3600>>(h);
      }

      //***********************************************************************
      /// Literal for minutes duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::minutes operator ""_minutes(unsigned long long m) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::minutes operator ""_min(unsigned long long m) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::minutes(static_cast<etl::chrono::minutes::rep>(m));
      }

      //***********************************************************************
      /// Literal for floating point minutes duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, ratio<60>> operator ""_minutes(long double m) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, ratio<60>> operator ""_min(long double m) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::duration<double, ratio<60>>(m);
      }

      //***********************************************************************
      /// Literal for seconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::seconds operator ""_seconds(unsigned long long s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::seconds operator ""_s(unsigned long long s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::seconds(static_cast<etl::chrono::seconds::rep>(s));
      }

      //***********************************************************************
      /// Literal for floating point seconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::duration<double> operator ""_seconds(long double s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::duration<double> operator ""_s(long double s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::duration<double>(s);
      }

      //***********************************************************************
      /// Literal for milliseconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::milliseconds operator ""_milliseconds(unsigned long long s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::milliseconds operator ""_ms(unsigned long long s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::milliseconds(static_cast<etl::chrono::milliseconds::rep>(s));
      }

      //***********************************************************************
      /// Literal for floating point milliseconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, milli> operator ""_milliseconds(long double s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, milli> operator ""_ms(long double s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::duration<double, milli>(s);
      }

      //***********************************************************************
      /// Literal for microseconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::microseconds operator ""_microseconds(unsigned long long s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::microseconds operator ""_us(unsigned long long s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::microseconds(static_cast<etl::chrono::microseconds::rep>(s));
      }

      //***********************************************************************
      /// Literal for floating point microseconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, micro> operator ""_microseconds(long double s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, micro> operator ""_us(long double s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::duration<double, micro>(s);
      }

      //***********************************************************************
      /// Literal for nanoseconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::nanoseconds operator ""_nanoseconds(unsigned long long s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::nanoseconds operator ""_ns(unsigned long long s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::nanoseconds(static_cast<etl::chrono::nanoseconds::rep>(s));
      }

      //***********************************************************************
      /// Literal for floating point microseconds duration
      //***********************************************************************
#if ETL_USING_VERBOSE_CHRONO_LITERALS
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, nano> operator ""_nanoseconds(long double s) ETL_NOEXCEPT
#else
      inline ETL_CONSTEXPR14 etl::chrono::duration<double, nano> operator ""_ns(long double s) ETL_NOEXCEPT
#endif
      {
        return etl::chrono::duration<double, nano>(s);
      }
    }
  }
}
#endif
