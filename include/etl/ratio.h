///\file

/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2016 John Wellbelove

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

#ifndef ETL_RATIO_INCLUDED
#define ETL_RATIO_INCLUDED

#include "platform.h"
#include "static_assert.h"
#include "gcd.h"

#include "type_traits.h"

#include <stddef.h>
#include <stdint.h>

///\defgroup ratio ratio
///\ingroup maths

namespace etl
{
  //***********************************************************************
  /// ratio
  //***********************************************************************
  template <intmax_t Num, intmax_t Den = 1UL>
  struct ratio
  {
    ETL_STATIC_ASSERT(Num != 0, "Numerator cannot be zero");
    ETL_STATIC_ASSERT(Den != 0, "Denominator cannot be zero");

    static ETL_CONSTANT intmax_t num = Num / etl::gcd_const<Num, Den>::value;
    static ETL_CONSTANT intmax_t den = Den / etl::gcd_const<Num, Den>::value;

    typedef etl::ratio<num, den> type;
  };

  template <intmax_t Num, intmax_t Den>
  ETL_CONSTANT intmax_t ratio<Num, Den>::num;

  template <intmax_t Num, intmax_t Den>
  ETL_CONSTANT intmax_t ratio<Num, Den>::den;

#if ETL_USING_CPP11
  //***********************************************************************
  /// ratio_add
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  using ratio_add = etl::ratio<(TRatio1::num * TRatio2::den) + (TRatio2::num * TRatio1::den), TRatio1::den * TRatio2::den>;

  //***********************************************************************
  /// ratio_subtract
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  using ratio_subtract = etl::ratio<(TRatio1::num * TRatio2::den) - (TRatio2::num * TRatio1::den), TRatio1::den * TRatio2::den>;

  //***********************************************************************
  /// ratio_multiply
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  using ratio_multiply = etl::ratio<TRatio1::num * TRatio2::num, TRatio1::den * TRatio2::den>;

  //***********************************************************************
  /// ratio_divide
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  using ratio_divide = etl::ratio<TRatio1::num * TRatio2::den, TRatio1::den * TRatio2::num>;
#endif

  //***********************************************************************
  /// ratio_equal
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  struct ratio_equal : etl::bool_constant<(TRatio1::num == TRatio2::num) && (TRatio1::den == TRatio2::den)>
  {
  };

  //***********************************************************************
  /// ratio_not_equal
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  struct ratio_not_equal : etl::bool_constant<!etl::ratio_equal<TRatio1, TRatio2>::value>
  {
  };

  //***********************************************************************
  /// ratio_less
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  struct ratio_less : etl::bool_constant<(TRatio1::num * TRatio2::den) < (TRatio2::num * TRatio1::den)>
  {
  };

  //***********************************************************************
  /// ratio_less_equal
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  struct ratio_less_equal : etl::bool_constant<!etl::ratio_less<TRatio2, TRatio1>::value>
  {
  };

  //***********************************************************************
  /// ratio_greater
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  struct ratio_greater : etl::bool_constant<etl::ratio_less<TRatio2, TRatio1>::value>
  {
  };

  //***********************************************************************
  /// ratio_greater_equal
  //***********************************************************************
  template <typename TRatio1, typename TRatio2>
  struct ratio_greater_equal : etl::bool_constant<!etl::ratio_less<TRatio1, TRatio2>::value>
  {
  };

#if ETL_USING_CPP17
  template<typename R1, typename R2>
  inline constexpr bool ratio_equal_v = ratio_equal<R1, R2>::value;

  template<typename R1, typename R2>
  inline constexpr bool ratio_not_equal_v = ratio_not_equal<R1, R2>::value;

  template<typename R1, typename R2>
  inline constexpr bool ratio_less_v = ratio_less<R1, R2>::value;

  template<typename R1, typename R2>
  inline constexpr bool ratio_less_equal_v = ratio_less_equal<R1, R2>::value;

  template<typename R1, typename R2>
  inline constexpr bool ratio_greater_v = ratio_greater<R1, R2>::value;

  template<typename R1, typename R2>
  inline constexpr bool ratio_greater_equal_v = ratio_greater_equal<R1, R2>::value;
#endif

  //***********************************************************************
  /// Predefined ration types.
  //***********************************************************************
  #if INT_MAX > INT32_MAX
    typedef ratio<1, 1000000000000000000>       atto;
    typedef ratio<1, 1000000000000000>          femto;
    typedef ratio<1, 1000000000000>             pico;
  #endif

  #if (INT_MAX >= INT32_MAX)
    typedef ratio<1, 1000000000>                nano;
    typedef ratio<1, 1000000>                   micro;
  #endif

  #if (INT_MAX >= INT16_MAX)
    typedef ratio<1, 1000>                      milli;
    typedef ratio<1, 100>                       centi;
    typedef ratio<1, 10>                        deci;
    typedef ratio<10, 1>                        deca;
    typedef ratio<100, 1>                       hecto;
    typedef ratio<1000, 1>                      kilo;
  #endif

  #if (INT_MAX >= INT32_MAX)
    typedef ratio<1000000, 1>                   mega;
    typedef ratio<1000000000, 1>                giga;
  #endif

  #if INT_MAX > INT32_MAX
    typedef ratio<1000000000000, 1>             tera;
    typedef ratio<1000000000000000, 1>          peta;
    typedef ratio<1000000000000000000, 1>       exa;
  #endif

  /// An approximation of Pi.
  typedef ratio<355, 113> ratio_pi;

  /// An approximation of root 2.
  typedef ratio<239, 169> ratio_root2;

  /// An approximation of 1 over root 2.
  typedef ratio<169, 239> ratio_1_over_root2;

  /// An approximation of e.
  typedef ratio<326, 120> ratio_e;

#if ETL_USING_CPP11
  namespace private_ratio
  {
    // Primary template for GCD calculation
    template <typename T, T Value1, T Value2, bool = (Value2 == 0)>
    struct ratio_gcd;

    // Specialisation for the case when Value2 is not zero
    template <typename T, T Value1, T Value2>
    struct ratio_gcd<T, Value1, Value2, false>
    {
      static constexpr T value = ratio_gcd<T, Value2, Value1 % Value2>::value;
    };

    // Specialisation for the case when Value2 is zero
    template <typename T, T Value1, T Value2>
    struct ratio_gcd<T, Value1, Value2, true>
    {
      static constexpr T value = (Value1 < 0) ? -Value1 : Value1;
    };

    // Primary template for LCM calculation
    template <typename T, T Value1, T Value2>
    struct ratio_lcm
    {
    private:

      static constexpr T product = ((Value1 * Value2) < 0) ? -(Value1 * Value2) : Value1 * Value2;

    public:

      static constexpr T value = product / ratio_gcd<T, Value1, Value2>::value;
    };

    template<typename R1>
    struct ratio_reduce
    {
    private:

      static ETL_CONSTEXPR11 intmax_t gcd = etl::private_ratio::ratio_gcd<intmax_t, R1::num, R1::den>::value;

    public:

      using type = ratio<R1::num / gcd, R1::den / gcd>;
    };

    template<typename R1, typename R2>
    struct ratio_add
    {
    private:

      static ETL_CONSTEXPR11 intmax_t lcm = etl::private_ratio::ratio_lcm<intmax_t, R1::den, R2::den>::value;

    public:

      using type = typename ratio_reduce<ratio<R1::num * lcm / R1::den + R2::num * lcm / R2::den, lcm>>::type;
    };

    template<typename R1, typename R2>
    struct ratio_subtract
    {
    public:
      using type = typename ratio_add<R1, ratio<-R2::num, R2::den>>::type;
    };

    template<typename R1, typename R2>
    struct ratio_multiply
    {
    private:
      static ETL_CONSTEXPR11 intmax_t gcd1 = etl::private_ratio::ratio_gcd<intmax_t, R1::num, R2::den>::value;
      static ETL_CONSTEXPR11 intmax_t gcd2 = etl::private_ratio::ratio_gcd<intmax_t, R2::num, R1::den>::value;

    public:
      using type = ratio<(R1::num / gcd1) * (R2::num / gcd2), (R1::den / gcd2) * (R2::den / gcd1)>;
    };

    template<typename R1, typename R2>
    struct ratio_divide
    {
    public:
      using type = typename ratio_multiply<R1, ratio<R2::den, R2::num>>::type;
    };
  }

#endif
}

#endif

