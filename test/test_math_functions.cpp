/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 John Wellbelove

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

#include "etl/log.h"
#include "etl/power.h"
#include "etl/fibonacci.h"
#include "etl/factorial.h"
#include "etl/sqrt.h"
#include "etl/permutations.h"
#include "etl/combinations.h"
#include "etl/gcd.h"
#include "etl/lcm.h"

namespace
{
  int sqrt(int v)
  {
    int i = 1;

    while (i < v)
    {
      if ((i * i) > v)
      {
        return i - 1;
      }

      ++i;
    }

    return 1;
  }

  size_t factorial(size_t n)
  {
    if (n == 1)
    {
      return 1;
    }
    else
    {
      return n * factorial(n - 1);
    }
  }

  size_t permutations(size_t n, size_t k)
  {
    size_t p = 1;

    for (size_t i = 0UL; i < k; ++i)
    {
      p *= n;
      --n;
    }

    return p;
  }

  size_t combinations(size_t n, size_t k)
  {
    return permutations(n, k) / factorial(k);
  }

  SUITE(test_math_functions)
  {
    //*************************************************************************
    TEST(test_log_0_base)
    {
      int actual;

      actual = etl::log<0, 2>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log<0, 10>::value;
      CHECK_EQUAL(0, actual);
    }

    //*************************************************************************
    TEST(test_log_1_base)
    {
      int actual;

      actual = etl::log<1, 2>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log<1, 10>::value;
      CHECK_EQUAL(0, actual);
    }

    //*************************************************************************
    TEST(test_log_10_base)
    {
      int actual;

      actual = etl::log<10, 2>::value;
      CHECK_EQUAL(3, actual);

      actual = etl::log<10, 10>::value;
      CHECK_EQUAL(1, actual);
    }

    //*************************************************************************
    TEST(test_log_100_base)
    {
      int actual;

      actual = etl::log<100, 2>::value;
      CHECK_EQUAL(6, actual);

      actual = etl::log<100, 10>::value;
      CHECK_EQUAL(2, actual);
    }

    //*************************************************************************
    TEST(test_log_1000_base)
    {
      int actual;

      actual = etl::log<1000, 2>::value;
      CHECK_EQUAL(9, actual);

      actual = etl::log<1000, 10>::value;
      CHECK_EQUAL(3, actual);
    }

    //*************************************************************************
    TEST(test_log_2)
    {
      int actual;

      actual = etl::log2<0>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log2<1>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log2<10>::value;
      CHECK_EQUAL(3, actual);

      actual = etl::log2<100>::value;
      CHECK_EQUAL(6, actual);
    }

    //*************************************************************************
    TEST(test_log_10)
    {
      int actual;

      actual = etl::log10<0>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log10<1>::value;
      CHECK_EQUAL(0, actual);

      actual = etl::log10<10>::value;
      CHECK_EQUAL(1, actual);

      actual = etl::log10<100>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::log10<200>::value;
      CHECK_EQUAL(2, actual);
    }

    //*************************************************************************
    TEST(test_power)
    {
      uint64_t actual;

      // 2^1
      actual = etl::power<2, 1>::value;
      CHECK_EQUAL(2U, actual);

      // 3^2
      actual = etl::power<3, 2>::value;
      CHECK_EQUAL(9U, actual);

      // 4^3
      actual = etl::power<4, 3>::value;
      CHECK_EQUAL(64U, actual);

      // 5^4
      actual = etl::power<5, 4>::value;
      CHECK_EQUAL(625U, actual);

      // 6^5
      actual = etl::power<6, 5>::value;
      CHECK_EQUAL(7776U, actual);

      // 7^6
      actual = etl::power<7, 6>::value;
      CHECK_EQUAL(117649UL, actual);

      // 8^7
      actual = etl::power<8, 7>::value;
      CHECK_EQUAL(2097152UL, actual);

      // 9^8
      actual = etl::power<9, 8>::value;
      CHECK_EQUAL(43046721UL, actual);

      // 10^9
      actual = etl::power<10, 9>::value;
      CHECK_EQUAL(1000000000UL, actual);

      // 2^16
      actual = etl::power<2, 15>::value;
      CHECK_EQUAL(0x8000U, actual);

      // 2^31
      actual = etl::power<2, 31>::value;
      CHECK_EQUAL(0x80000000UL, actual);

      // 2^63
      actual = etl::power<2, 63>::value;
      CHECK_EQUAL(0x8000000000000000ULL, actual);
    }

    //*************************************************************************
    TEST(test_power_of_2_round_up)
    {
      int actual;

      //
      actual = etl::power_of_2_round_up<0>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_up<1>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_up<2>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_up<3>::value;
      CHECK_EQUAL(4, actual);

      actual = etl::power_of_2_round_up<4>::value;
      CHECK_EQUAL(4, actual);

      actual = etl::power_of_2_round_up<5>::value;
      CHECK_EQUAL(8, actual);

      actual = etl::power_of_2_round_up<127>::value;
      CHECK_EQUAL(128, actual);

      actual = etl::power_of_2_round_up<128>::value;
      CHECK_EQUAL(128, actual);

      actual = etl::power_of_2_round_up<129>::value;
      CHECK_EQUAL(256, actual);
    }

    //*************************************************************************
    TEST(test_power_of_2_round_down)
    {
      int actual;

      actual = etl::power_of_2_round_down<0>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_down<1>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_down<2>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_down<3>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_down<4>::value;
      CHECK_EQUAL(2, actual);

      actual = etl::power_of_2_round_down<5>::value;
      CHECK_EQUAL(4, actual);

      actual = etl::power_of_2_round_down<127>::value;
      CHECK_EQUAL(64, actual);

      actual = etl::power_of_2_round_down<128>::value;
      CHECK_EQUAL(64, actual);

      actual = etl::power_of_2_round_down<129>::value;
      CHECK_EQUAL(128, actual);
    }

    //*************************************************************************
    TEST(test_is_power_of_2)
    {
      bool actual;

      actual = etl::is_power_of_2<0>::value;
      CHECK_EQUAL(false, actual);

      actual = etl::is_power_of_2<1>::value;
      CHECK_EQUAL(false, actual);

      actual = etl::is_power_of_2<2>::value;
      CHECK_EQUAL(true, actual);

      actual = etl::is_power_of_2<3>::value;
      CHECK_EQUAL(false, actual);

      actual = etl::is_power_of_2<4>::value;
      CHECK_EQUAL(true, actual);

      actual = etl::is_power_of_2<5>::value;
      CHECK_EQUAL(false, actual);

      actual = etl::is_power_of_2<127>::value;
      CHECK_EQUAL(false, actual);

      actual = etl::is_power_of_2<128>::value;
      CHECK_EQUAL(true, actual);

      actual = etl::is_power_of_2<129>::value;
      CHECK_EQUAL(false, actual);
    }

    //*************************************************************************
    TEST(test_fibonacci)
    {
      CHECK_EQUAL(0U,          (size_t)etl::fibonacci<0>::value);
      CHECK_EQUAL(1U,          (size_t)etl::fibonacci<1>::value);
      CHECK_EQUAL(1U,          (size_t)etl::fibonacci<2>::value);
      CHECK_EQUAL(2U,          (size_t)etl::fibonacci<3>::value);
      CHECK_EQUAL(3U,          (size_t)etl::fibonacci<4>::value);
      CHECK_EQUAL(5U,          (size_t)etl::fibonacci<5>::value);
      CHECK_EQUAL(8U,          (size_t)etl::fibonacci<6>::value);
      CHECK_EQUAL(13U,         (size_t)etl::fibonacci<7>::value);
      CHECK_EQUAL(21U,         (size_t)etl::fibonacci<8>::value);
      CHECK_EQUAL(34U,         (size_t)etl::fibonacci<9>::value);
      CHECK_EQUAL(55U,         (size_t)etl::fibonacci<10>::value);
      CHECK_EQUAL(89U,         (size_t)etl::fibonacci<11>::value);
      CHECK_EQUAL(144U,        (size_t)etl::fibonacci<12>::value);
      CHECK_EQUAL(233U,        (size_t)etl::fibonacci<13>::value);
      CHECK_EQUAL(377U,        (size_t)etl::fibonacci<14>::value);
      CHECK_EQUAL(610U,        (size_t)etl::fibonacci<15>::value);
      CHECK_EQUAL(987U,        (size_t)etl::fibonacci<16>::value);
      CHECK_EQUAL(1597U,       (size_t)etl::fibonacci<17>::value);
      CHECK_EQUAL(2584U,       (size_t)etl::fibonacci<18>::value);
      CHECK_EQUAL(4181U,       (size_t)etl::fibonacci<19>::value);
      CHECK_EQUAL(6765U,       (size_t)etl::fibonacci<20>::value);
      CHECK_EQUAL(10946U,      (size_t)etl::fibonacci<21>::value);
      CHECK_EQUAL(17711U,      (size_t)etl::fibonacci<22>::value);
      CHECK_EQUAL(28657U,      (size_t)etl::fibonacci<23>::value);
      CHECK_EQUAL(46368U,      (size_t)etl::fibonacci<24>::value);
      CHECK_EQUAL(75025U,      (size_t)etl::fibonacci<25>::value);
      CHECK_EQUAL(121393U,     (size_t)etl::fibonacci<26>::value);
      CHECK_EQUAL(196418U,     (size_t)etl::fibonacci<27>::value);
      CHECK_EQUAL(317811U,     (size_t)etl::fibonacci<28>::value);
      CHECK_EQUAL(514229U,     (size_t)etl::fibonacci<29>::value);
      CHECK_EQUAL(832040U,     (size_t)etl::fibonacci<30>::value);
      CHECK_EQUAL(1346269U,    (size_t)etl::fibonacci<31>::value);
      CHECK_EQUAL(2178309U,    (size_t)etl::fibonacci<32>::value);
      CHECK_EQUAL(3524578U,    (size_t)etl::fibonacci<33>::value);
      CHECK_EQUAL(5702887U,    (size_t)etl::fibonacci<34>::value);
      CHECK_EQUAL(9227465U,    (size_t)etl::fibonacci<35>::value);
      CHECK_EQUAL(14930352U,   (size_t)etl::fibonacci<36>::value);
      CHECK_EQUAL(24157817U,   (size_t)etl::fibonacci<37>::value);
      CHECK_EQUAL(39088169U,   (size_t)etl::fibonacci<38>::value);
      CHECK_EQUAL(63245986U,   (size_t)etl::fibonacci<39>::value);
      CHECK_EQUAL(102334155U,  (size_t)etl::fibonacci<40>::value);
      CHECK_EQUAL(165580141U,  (size_t)etl::fibonacci<41>::value);
      CHECK_EQUAL(267914296U,  (size_t)etl::fibonacci<42>::value);
      CHECK_EQUAL(433494437U,  (size_t)etl::fibonacci<43>::value);
      CHECK_EQUAL(701408733U,  (size_t)etl::fibonacci<44>::value);
      CHECK_EQUAL(1134903170U, (size_t)etl::fibonacci<45>::value);
      CHECK_EQUAL(1836311903U, (size_t)etl::fibonacci<46>::value);
      CHECK_EQUAL(2971215073U, (size_t)etl::fibonacci<47>::value);
    }

    TEST(test_factorial)
    {
      CHECK_EQUAL(1U,         (size_t)etl::factorial<0>::value);
      CHECK_EQUAL(1U,         (size_t)etl::factorial<1>::value);
      CHECK_EQUAL(2U,         (size_t)etl::factorial<2>::value);
      CHECK_EQUAL(6U,         (size_t)etl::factorial<3>::value);
      CHECK_EQUAL(24U,        (size_t)etl::factorial<4>::value);
      CHECK_EQUAL(120U,       (size_t)etl::factorial<5>::value);
      CHECK_EQUAL(720U,       (size_t)etl::factorial<6>::value);
      CHECK_EQUAL(5040U,      (size_t)etl::factorial<7>::value);
      CHECK_EQUAL(40320U,     (size_t)etl::factorial<8>::value);
      CHECK_EQUAL(362880U,    (size_t)etl::factorial<9>::value);
      CHECK_EQUAL(3628800U,   (size_t)etl::factorial<10>::value);
      CHECK_EQUAL(39916800U,  (size_t)etl::factorial<11>::value);
      CHECK_EQUAL(479001600U, (size_t)etl::factorial<12>::value);
    }

    //*************************************************************************
    TEST(test_sqrt)
    {
      CHECK_EQUAL(sqrt(1),  etl::sqrt<1>::value);
      CHECK_EQUAL(sqrt(2),  etl::sqrt<2>::value);
      CHECK_EQUAL(sqrt(3),  etl::sqrt<3>::value);
      CHECK_EQUAL(sqrt(4),  etl::sqrt<4>::value);
      CHECK_EQUAL(sqrt(5),  etl::sqrt<5>::value);
      CHECK_EQUAL(sqrt(6),  etl::sqrt<6>::value);
      CHECK_EQUAL(sqrt(7),  etl::sqrt<7>::value);
      CHECK_EQUAL(sqrt(8),  etl::sqrt<8>::value);
      CHECK_EQUAL(sqrt(9),  etl::sqrt<9>::value);
      CHECK_EQUAL(sqrt(10), etl::sqrt<10>::value);
    }

    //*************************************************************************
    TEST(test_permutations)
    {
      size_t actual;


      CHECK_EQUAL((permutations( 6,  2)), (actual = etl::permutations< 6,  2>::value));
      CHECK_EQUAL((permutations( 7,  3)), (actual = etl::permutations< 7,  3>::value));
      CHECK_EQUAL((permutations( 8,  4)), (actual = etl::permutations< 8,  4>::value));
      CHECK_EQUAL((permutations( 9,  5)), (actual = etl::permutations< 9,  5>::value));
      CHECK_EQUAL((permutations(10,  6)), (actual = etl::permutations<10,  6>::value));
      CHECK_EQUAL((permutations(11,  7)), (actual = etl::permutations<11,  7>::value));
      CHECK_EQUAL((permutations(12,  8)), (actual = etl::permutations<12,  8>::value));
      CHECK_EQUAL((permutations(13,  9)), (actual = etl::permutations<13,  9>::value));
      CHECK_EQUAL((permutations(14, 10)), (actual = etl::permutations<14, 10>::value));
    }

    //*************************************************************************
    TEST(test_combinations)
    {
      size_t actual;

      CHECK_EQUAL((combinations( 6,  2)), (actual = etl::combinations< 6,  2>::value));
      CHECK_EQUAL((combinations( 7,  3)), (actual = etl::combinations< 7,  3>::value));
      CHECK_EQUAL((combinations( 8,  4)), (actual = etl::combinations< 8,  4>::value));
      CHECK_EQUAL((combinations( 9,  5)), (actual = etl::combinations< 9,  5>::value));
      CHECK_EQUAL((combinations(10,  6)), (actual = etl::combinations<10,  6>::value));
      CHECK_EQUAL((combinations(11,  7)), (actual = etl::combinations<11,  7>::value));
      CHECK_EQUAL((combinations(12,  8)), (actual = etl::combinations<12,  8>::value));
      CHECK_EQUAL((combinations(13,  9)), (actual = etl::combinations<13,  9>::value));
      CHECK_EQUAL((combinations(14, 10)), (actual = etl::combinations<14, 10>::value));
    }

    //*************************************************************************
    TEST(test_gdc_for_positive_integers)
    {
      CHECK_EQUAL(1, etl::gcd( 3, 5,  7));
      CHECK_EQUAL(2, etl::gcd( 4, 6,  8));
      CHECK_EQUAL(3, etl::gcd( 9, 6,  3));
      CHECK_EQUAL(4, etl::gcd( 8, 4, 12));
      CHECK_EQUAL(5, etl::gcd(10, 5, 15));
    }

    //*************************************************************************
    TEST(test_gdc_for_negative_integers)
    {
      CHECK_EQUAL(1, etl::gcd( -3, -5, -7));
      CHECK_EQUAL(2, etl::gcd( -4, -6, -8));
      CHECK_EQUAL(3, etl::gcd( -9, -6, -3));
      CHECK_EQUAL(4, etl::gcd( -8, -4, -12));
      CHECK_EQUAL(5, etl::gcd(-10, -5, -15));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_gdc)
    {
      constexpr int gcd1 = etl::gcd(3, 5, 7);
      constexpr int gcd2 = etl::gcd(4, 6, 8);
      constexpr int gcd3 = etl::gcd(9, 6, 3);
      constexpr int gcd4 = etl::gcd(8, 4, 12);
      constexpr int gcd5 = etl::gcd(10, 5, 15);

      CHECK_EQUAL(1, gcd1);
      CHECK_EQUAL(2, gcd2);
      CHECK_EQUAL(3, gcd3);
      CHECK_EQUAL(4, gcd4);
      CHECK_EQUAL(5, gcd5);
    }
#endif

    //*************************************************************************
    TEST(test_lcm_for_positive_integers) 
    {
      CHECK_EQUAL(12, etl::lcm(3, 4,  6));
      CHECK_EQUAL(30, etl::lcm(3, 5, 10));
      CHECK_EQUAL(20, etl::lcm(2, 5,  4));
      CHECK_EQUAL(0,  etl::lcm(0, 5,  7));
      CHECK_EQUAL(5,  etl::lcm(5, 5,  5));
    }

    //*************************************************************************
    TEST(test_lcm_for_negative_integers)
    {
      CHECK_EQUAL(12, etl::lcm(-3, -4,  -6));
      CHECK_EQUAL(30, etl::lcm(-3, -5, -10));
      CHECK_EQUAL(20, etl::lcm(-2, -5,  -4));
      CHECK_EQUAL(0,  etl::lcm(-0, -5,  -7));
      CHECK_EQUAL(5,  etl::lcm(-5, -5,  -5));
    }

#if ETL_USING_CPP14
    //*************************************************************************
    TEST(test_constexpr_lcm)
    {
      constexpr int lcm1 = etl::lcm(3, 4, 6);
      constexpr int lcm2 = etl::lcm(3, 5, 10);
      constexpr int lcm3 = etl::lcm(2, 5, 4);
      constexpr int lcm4 = etl::lcm(0, 5, 7);
      constexpr int lcm5 = etl::lcm(5, 5, 5);

      CHECK_EQUAL(12, lcm1);
      CHECK_EQUAL(30, lcm2);
      CHECK_EQUAL(20, lcm3);
      CHECK_EQUAL(0,  lcm4);
      CHECK_EQUAL(5,  lcm5);
    }
#endif
  };
}
