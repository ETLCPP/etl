/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Documentation: 

Copyright(c) 2024 John Wellbelove

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

#include "unit_test_framework.h"

#include "etl/ratio.h"

namespace
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

  SUITE(test_ratio)
  {
    //*************************************************************************
    TEST(test_definitions)
    {
      constexpr intmax_t Num = 20;
      constexpr intmax_t Den = 600;

      intmax_t N = Num / gcd<Num, Den>::value;
      intmax_t D = Den / gcd<Num, Den>::value;

      using ratio      = etl::ratio<Num, Den>;
      using ratio_type = ratio::type;

      CHECK_FALSE((std::is_same<ratio, ratio_type>::value));

      CHECK_EQUAL(N, ratio::num);
      CHECK_EQUAL(D, ratio::den);

      CHECK_EQUAL(N, ratio_type::num);
      CHECK_EQUAL(D, ratio_type::den);
    }

    //*************************************************************************
    TEST(test_ratio_add)
    {
      using two_thirds = etl::ratio<2, 3>;
      using one_sixth  = etl::ratio<1, 6>;
      using ratio      = etl::ratio_add<two_thirds, one_sixth>;

      CHECK_TRUE((std::is_same<ratio, etl::ratio<15, 18>>::value));
      CHECK_EQUAL(5, ratio::num);
      CHECK_EQUAL(6, ratio::den);
    }

    //*************************************************************************
    TEST(test_ratio_subtract)
    {
      using two_thirds = etl::ratio<2, 3>;
      using one_sixth  = etl::ratio<1, 6>;
      using ratio      = etl::ratio_subtract<two_thirds, one_sixth>;

      CHECK_TRUE((std::is_same<ratio, etl::ratio<9, 18>>::value));
      CHECK_EQUAL(1, ratio::num);
      CHECK_EQUAL(2, ratio::den);
    }

    //*************************************************************************
    TEST(test_ratio_multiply)
    {
      using two_thirds = etl::ratio<2, 3>;
      using one_sixth  = etl::ratio<1, 6>;
      using ratio      = etl::ratio_multiply<two_thirds, one_sixth>;

      CHECK_TRUE((std::is_same<ratio, etl::ratio<2, 18>>::value));
      CHECK_EQUAL(1, ratio::num);
      CHECK_EQUAL(9, ratio::den);
    }

    //*************************************************************************
    TEST(test_ratio_divide)
    {
      using two_thirds = etl::ratio<2, 3>;
      using one_sixth  = etl::ratio<1, 6>;
      using ratio      = etl::ratio_divide<two_thirds, one_sixth>;

      CHECK_TRUE((std::is_same<ratio, etl::ratio<12, 3>>::value));
      CHECK_EQUAL(4, ratio::num);
      CHECK_EQUAL(1, ratio::den);
    }

    //*************************************************************************
    TEST(test_ratio_equal)
    {
      using ratio1 = etl::ratio<5, 32>;
      using ratio2 = etl::ratio<3, 16>; // 6/32

      CHECK_TRUE((etl::ratio_equal<ratio1, ratio1>::value));
      CHECK_FALSE((etl::ratio_equal<ratio1, ratio2>::value));
      CHECK_FALSE((etl::ratio_equal<ratio2, ratio1>::value));
    }

    //*************************************************************************
    TEST(test_ratio_not_equal)
    {
      using ratio1 = etl::ratio<5, 32>;
      using ratio2 = etl::ratio<3, 16>; // 6/32

      CHECK_FALSE((etl::ratio_not_equal<ratio1, ratio1>::value));
      CHECK_TRUE((etl::ratio_not_equal<ratio1, ratio2>::value));
      CHECK_TRUE((etl::ratio_not_equal<ratio2, ratio1>::value));
    }

    //*************************************************************************
    TEST(test_ratio_less)
    {
      using ratio1 = etl::ratio<5, 32>;
      using ratio2 = etl::ratio<3, 16>; // 6/32

      CHECK_FALSE((etl::ratio_less<ratio1, ratio1>::value));
      CHECK_TRUE((etl::ratio_less<ratio1, ratio2>::value));
      CHECK_FALSE((etl::ratio_less<ratio2, ratio1>::value));
    }

    //*************************************************************************
    TEST(test_ratio_less_equal)
    {
      using ratio1 = etl::ratio<5, 32>;
      using ratio2 = etl::ratio<3, 16>; // 6/32

      CHECK_TRUE((etl::ratio_less_equal<ratio1, ratio1>::value));
      CHECK_TRUE((etl::ratio_less_equal<ratio1, ratio2>::value));
      CHECK_FALSE((etl::ratio_less_equal<ratio2, ratio1>::value));
    }

    //*************************************************************************
    TEST(test_ratio_greater)
    {
      using ratio1 = etl::ratio<5, 32>;
      using ratio2 = etl::ratio<3, 16>; // 6/32

      CHECK_FALSE((etl::ratio_greater<ratio1, ratio1>::value));
      CHECK_FALSE((etl::ratio_greater<ratio1, ratio2>::value));
      CHECK_TRUE((etl::ratio_greater<ratio2, ratio1>::value));
    }

    //*************************************************************************
    TEST(test_ratio_greater_equal)
    {
      using ratio1 = etl::ratio<5, 32>;
      using ratio2 = etl::ratio<3, 16>; // 6/32

      CHECK_TRUE((etl::ratio_greater_equal<ratio1, ratio1>::value));
      CHECK_FALSE((etl::ratio_greater_equal<ratio1, ratio2>::value));
      CHECK_TRUE((etl::ratio_greater_equal<ratio2, ratio1>::value));
    }

    //*************************************************************************
    TEST(test_predefined_ratios)
    {
#if INT_MAX > INT32_MAX
      CHECK_EQUAL(etl::atto::num, 1);
      CHECK_EQUAL(etl::atto::den, 1000000000000000000);
      CHECK_EQUAL(etl::femto::num, 1);
      CHECK_EQUAL(etl::femto::den, 1000000000000000);
      CHECK_EQUAL(etl::pico::num, 1);
      CHECK_EQUAL(etl::pico::den, 1000000000000);
#endif
#if (INT_MAX >= INT32_MAX)
      CHECK_EQUAL(etl::nano::num, 1);
      CHECK_EQUAL(etl::nano::den, 1000000000);
      CHECK_EQUAL(etl::micro::num, 1);
      CHECK_EQUAL(etl::micro::den, 1000000);
#endif
#if (INT_MAX >= INT16_MAX)
      CHECK_EQUAL(etl::milli::num, 1);
      CHECK_EQUAL(etl::milli::den, 1000);
      CHECK_EQUAL(etl::centi::num, 1);
      CHECK_EQUAL(etl::centi::den, 100);
      CHECK_EQUAL(etl::deci::num, 1);
      CHECK_EQUAL(etl::deci::den, 10);
      CHECK_EQUAL(etl::deca::num, 10);
      CHECK_EQUAL(etl::deca::den, 1);
      CHECK_EQUAL(etl::hecto::num, 100);
      CHECK_EQUAL(etl::hecto::den, 1);
      CHECK_EQUAL(etl::kilo::num, 1000);
      CHECK_EQUAL(etl::kilo::den, 1);
#endif
#if (INT_MAX >= INT32_MAX)
      CHECK_EQUAL(etl::mega::num, 1000000);
      CHECK_EQUAL(etl::mega::den, 1);
      CHECK_EQUAL(etl::giga::num, 1000000000);
      CHECK_EQUAL(etl::giga::den, 1);
#endif

#if INT_MAX > INT32_MAX
      CHECK_EQUAL(etl::tera::num, 1000000000000);
      CHECK_EQUAL(etl::tera::den, 1);
      CHECK_EQUAL(etl::peta::num, 1000000000000000);
      CHECK_EQUAL(etl::peta::den, 1);
      CHECK_EQUAL(etl::exa::num, 1000000000000000000);
      CHECK_EQUAL(etl::exa::den, 1);
#endif

      double expected_pi = 3.1415926535897931;
      double actual_pi   = double(etl::ratio_pi::num) / double(etl::ratio_pi::den);
      CHECK_CLOSE(expected_pi, actual_pi, 0.0000003);

      double expected_root_2 = 1.414213562373095;
      double actual_root_2   = double(etl::ratio_root2::num) / double(etl::ratio_root2::den);
      CHECK_CLOSE(expected_root_2, actual_root_2, 0.00002);

      double expected_1_over_root_2 = 1.0 / 1.414213562373095;
      double actual_1_over_root_2   = double(etl::ratio_1_over_root2::num) / double(etl::ratio_1_over_root2::den);
      CHECK_CLOSE(expected_1_over_root_2, actual_1_over_root_2, 0.000007);

      double expected_e = 2.7182818284590451;
      double actual_e   = double(etl::ratio_e::num) / double(etl::ratio_e::den);
      CHECK_CLOSE(expected_e, actual_e, 0.0004);
    }
  };
}
