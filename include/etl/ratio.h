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
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#ifndef ETL_RATIO_INCLUDED
#define ETL_RATIO_INCLUDED

#include "platform.h"

#include "type_traits.h"

#include <stddef.h>
#include <stdint.h>

///\defgroup ratio ratio
///\ingroup maths

namespace etl
{
  template <intmax_t NUM, intmax_t DEN = 1UL>
  struct ratio
  {
    static ETL_CONSTANT intmax_t num = NUM;
    static ETL_CONSTANT intmax_t den = DEN;
  };

  template <intmax_t NUM, intmax_t DEN>
  ETL_CONSTANT intmax_t ratio<NUM, DEN>::num;

  template <intmax_t NUM, intmax_t DEN>
  ETL_CONSTANT intmax_t ratio<NUM, DEN>::den;

  #if INT_MAX > INT32_MAX
    typedef ratio<1, 1000000000000000000000000> yocto;
    typedef ratio<1, 1000000000000000000000>    zepto;
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
    typedef ratio<1000000000000000000000, 1>    zetta;
    typedef ratio<1000000000000000000000000, 1> yotta;
  #endif

  /// An approximation of PI to 6 digits.
  typedef ratio<355, 113> ratio_pi;

  /// An approximation of root 2.
  typedef ratio<239, 169> ratio_root2;

  /// An approximation of e.
  typedef ratio<326, 120> ratio_e;

#if ETL_USING_CPP11
  namespace private_ratio
  {
    // Primary template for GCD calculation
    template <typename T, T A, T B, bool = (B == 0)>
    struct ratio_gcd;

    // Specialisation for the case when B is not zero
    template <typename T, T A, T B>
    struct ratio_gcd<T, A, B, false>
    {
      static constexpr T value = ratio_gcd<T, B, A % B>::value;
    };

    // Specialisation for the case when B is zero
    template <typename T, T A, T B>
    struct ratio_gcd<T, A, B, true>
    {
      static constexpr T value = (A < 0) ? -A : A;
    };

    // Primary template for LCM calculation
    template <typename T, T A, T B>
    struct ratio_lcm
    {
    private:

      static constexpr T product = ((A * B) < 0) ? -(A * B) : A * B;

    public:

      static constexpr T value = product / ratio_gcd<T, A, B>::value;
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

  template<typename R1, typename R2>
  using ratio_add = typename private_ratio::ratio_add<R1, R2>::type;

  template<typename R1, typename R2>
  using ratio_subtract = typename private_ratio::ratio_subtract<R1, R2>::type;

  template<typename R1, typename R2>
  using ratio_multiply = typename private_ratio::ratio_multiply<R1, R2>::type;

  template<typename R1, typename R2>
  using ratio_divide = typename private_ratio::ratio_divide<R1, R2>::type;

  template<typename R1, typename R2>
  struct ratio_equal : etl::integral_constant<bool, (R1::num == R2::num && R1::den == R2::den)>
  {
  };

  template<typename R1, typename R2>
  struct ratio_not_equal : etl::integral_constant<bool, (R1::num != R2::num || R1::den != R2::den)>
  {
  };

  template<typename R1, typename R2>
  struct ratio_less : etl::integral_constant<bool, (R1::num * R2::den < R2::num * R1::den)>
  {
  };

  template<typename R1, typename R2>
  struct ratio_less_equal : etl::integral_constant<bool, (R1::num * R2::den <= R2::num * R1::den)>
  {
  };

  template<typename R1, typename R2>
  struct ratio_greater : etl::integral_constant<bool, (R1::num * R2::den > R2::num * R1::den)>
  {
  };

  template<typename R1, typename R2>
  struct ratio_greater_equal: etl::integral_constant<bool, (R1::num * R2::den >= R2::num * R1::den)>
  {
  };

#if ETL_USING_CPP14
  template<typename R1, typename R2>
  ETL_CONSTEXPR14 bool ratio_equal_v = ratio_equal<R1, R2>::value;

  template<typename R1, typename R2>
  ETL_CONSTEXPR14 bool ratio_not_equal_v = ratio_not_equal<R1, R2>::value;

  template<typename R1, typename R2>
  ETL_CONSTEXPR14 bool ratio_less_v = ratio_less<R1, R2>::value;

  template<typename R1, typename R2>
  ETL_CONSTEXPR14 bool ratio_less_equal_v = ratio_less_equal<R1, R2>::value;

  template<typename R1, typename R2>
  ETL_CONSTEXPR14 bool ratio_greater_v = ratio_greater<R1, R2>::value;

  template<typename R1, typename R2>
  ETL_CONSTEXPR14 bool ratio_greater_equal_v = ratio_greater_equal<R1, R2>::value;
#endif
#endif
}

#endif

