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
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;
      using r3 = etl::ratio_subtract<r1, r2>;

      CHECK((etl::ratio_equal<r3, etl::ratio<-1, 6>>::value));
      CHECK((!etl::ratio_equal<r3, etl::ratio<-2, 6>>::value));
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
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;
      using r3 = etl::ratio_multiply<r1, r2>;

      CHECK((etl::ratio_equal<r3, etl::ratio<1, 3>>::value));
      CHECK((!etl::ratio_equal<r3, etl::ratio<1, 4>>::value));
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
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;
      using r3 = etl::ratio_divide<r1, r2>;

      CHECK((!etl::ratio_not_equal<r3, etl::ratio<3, 4>>::value));
      CHECK((etl::ratio_not_equal<r3, etl::ratio<4, 3>>::value));
    }

    //*************************************************************************
    TEST(test_ratio_equal)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_equal<r1, r1>::value));
      CHECK((!etl::ratio_equal<r1, r2>::value));

#if ETL_USING_CPP17
      CHECK((etl::ratio_equal_v<r1, r1>));
      CHECK((!etl::ratio_equal_v<r1, r2>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_not_equal)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((!etl::ratio_not_equal<r1, r1>::value));
      CHECK((etl::ratio_not_equal<r1, r2>::value));

#if ETL_USING_CPP17
      CHECK((!etl::ratio_not_equal_v<r1, r1>));
      CHECK((etl::ratio_not_equal_v<r1, r2>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_less)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_less<r1, r2>::value));
      CHECK((!etl::ratio_less<r2, r1>::value));

#if ETL_USING_CPP17
      CHECK((etl::ratio_less_v<r1, r2>));
      CHECK((!etl::ratio_less_v<r1, r1>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_less_equal)
    {
      using r1 = etl::ratio<1, 2>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_less_equal<r1, r1>::value));
      CHECK((etl::ratio_less_equal<r1, r2>::value));

#if ETL_USING_CPP17
      CHECK((etl::ratio_less_equal_v<r1, r1>));
      CHECK((etl::ratio_less_equal_v<r1, r1>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_greater)
    {
      using r1 = etl::ratio<4, 3>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_greater<r1, r2>::value));
      CHECK((!etl::ratio_greater<r2, r1>::value));

#if ETL_USING_CPP17
      CHECK((etl::ratio_greater_v<r1, r2>));
      CHECK((!etl::ratio_greater_v<r2, r1>));
#endif
    }

    //*************************************************************************
    TEST(test_ratio_greater_equal)
    {
      using r1 = etl::ratio<4, 3>;
      using r2 = etl::ratio<2, 3>;

      CHECK((etl::ratio_greater_equal<r1, r1>::value));
      CHECK((etl::ratio_greater_equal<r1, r2>::value));

#if ETL_USING_CPP17
      CHECK((etl::ratio_greater_equal_v<r1, r1>));
      CHECK((etl::ratio_greater_equal_v<r1, r2>));
#endif
    }
  };

}
