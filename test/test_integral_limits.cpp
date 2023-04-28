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

#include <limits>
#include <type_traits>
#include <bitset>

#include "etl/integral_limits.h"

namespace
{
  SUITE(test_integral_limits)
  {
    //*************************************************************************
    TEST(test_min)
    {
      CHECK_EQUAL(std::numeric_limits<char>::min(),               etl::integral_limits<char>::min);
      CHECK_EQUAL(std::numeric_limits<signed char>::min(),        (signed char)etl::integral_limits<signed char>::min);
      CHECK_EQUAL(std::numeric_limits<unsigned char>::min(),      (unsigned char)etl::integral_limits<unsigned char>::min);
      CHECK_EQUAL(std::numeric_limits<short>::min(),              (short)etl::integral_limits<short>::min);
      CHECK_EQUAL(std::numeric_limits<unsigned short>::min(),     (unsigned short)etl::integral_limits<unsigned short>::min);
      CHECK_EQUAL(std::numeric_limits<int>::min(),                (int)etl::integral_limits<int>::min);
      CHECK_EQUAL(std::numeric_limits<unsigned int>::min(),       (unsigned int)etl::integral_limits<unsigned int>::min);
      CHECK_EQUAL(std::numeric_limits<long>::min(),               (long)etl::integral_limits<long>::min);
      CHECK_EQUAL(std::numeric_limits<unsigned long>::min(),      (unsigned long)etl::integral_limits<unsigned long>::min);
      CHECK_EQUAL(std::numeric_limits<long long>::min(),          (long long)etl::integral_limits<long long>::min);
      CHECK_EQUAL(std::numeric_limits<unsigned long long>::min(), (unsigned long long)etl::integral_limits<unsigned long long>::min);
    }

    //*************************************************************************
    TEST(test_max)
    {
      CHECK_EQUAL(std::numeric_limits<char>::max(),               (char)etl::integral_limits<char>::max);
      CHECK_EQUAL(std::numeric_limits<signed char>::max(),        (signed char)etl::integral_limits<signed char>::max);
      CHECK_EQUAL(std::numeric_limits<unsigned char>::max(),      (unsigned char)etl::integral_limits<unsigned char>::max);
      CHECK_EQUAL(std::numeric_limits<short>::max(),              (short)etl::integral_limits<short>::max);
      CHECK_EQUAL(std::numeric_limits<unsigned short>::max(),     (unsigned short)etl::integral_limits<unsigned short>::max);
      CHECK_EQUAL(std::numeric_limits<int>::max(),                (int)etl::integral_limits<int>::max);
      CHECK_EQUAL(std::numeric_limits<unsigned int>::max(),       (unsigned int)etl::integral_limits<unsigned int>::max);
      CHECK_EQUAL(std::numeric_limits<long>::max(),               (long)etl::integral_limits<long>::max);
      CHECK_EQUAL(std::numeric_limits<unsigned long>::max(),      (unsigned long)etl::integral_limits<unsigned long>::max);
      CHECK_EQUAL(std::numeric_limits<long long>::max(),          (long long)etl::integral_limits<long long>::max);
      CHECK_EQUAL(std::numeric_limits<unsigned long long>::max(), (unsigned long long)etl::integral_limits<unsigned long long>::max);
    }

    //*************************************************************************
    TEST(test_bits)
    {
      CHECK_EQUAL(std::numeric_limits<std::make_unsigned<char>::type>::digits,        (int)etl::integral_limits<char>::bits);
      CHECK_EQUAL(std::numeric_limits<std::make_unsigned<signed char>::type>::digits, (int)etl::integral_limits<signed char>::bits);
      CHECK_EQUAL(std::numeric_limits<unsigned char>::digits,                         (int)etl::integral_limits<unsigned char>::bits);
      CHECK_EQUAL(std::numeric_limits<std::make_unsigned<short>::type>::digits,       (int)etl::integral_limits<short>::bits);
      CHECK_EQUAL(std::numeric_limits<unsigned short>::digits,                        (int)etl::integral_limits<unsigned short>::bits);
      CHECK_EQUAL(std::numeric_limits<std::make_unsigned<int>::type>::digits,         (int)etl::integral_limits<int>::bits);
      CHECK_EQUAL(std::numeric_limits<unsigned int>::digits,                          (int)etl::integral_limits<unsigned int>::bits);
      CHECK_EQUAL(std::numeric_limits<std::make_unsigned<long>::type>::digits,        (int)etl::integral_limits<long>::bits);
      CHECK_EQUAL(std::numeric_limits<unsigned long>::digits,                         (int)etl::integral_limits<unsigned long>::bits);
      CHECK_EQUAL(std::numeric_limits<std::make_unsigned<long long>::type>::digits,   (int)etl::integral_limits<long long>::bits);
      CHECK_EQUAL(std::numeric_limits<unsigned long long>::digits,                    (int)etl::integral_limits<unsigned long long>::bits);
    }

    //*************************************************************************
    TEST(test_is_signed)
    {
      CHECK_EQUAL(std::numeric_limits<char>::is_signed,               (bool)etl::integral_limits<char>::is_signed);
      CHECK_EQUAL(std::numeric_limits<signed char>::is_signed,        (bool)etl::integral_limits<signed char>::is_signed);
      CHECK_EQUAL(std::numeric_limits<unsigned char>::is_signed,      (bool)etl::integral_limits<unsigned char>::is_signed);
      CHECK_EQUAL(std::numeric_limits<short>::is_signed,              (bool)etl::integral_limits<short>::is_signed);
      CHECK_EQUAL(std::numeric_limits<unsigned short>::is_signed,     (bool)etl::integral_limits<unsigned short>::is_signed);
      CHECK_EQUAL(std::numeric_limits<int>::is_signed,                (bool)etl::integral_limits<int>::is_signed);
      CHECK_EQUAL(std::numeric_limits<unsigned int>::is_signed,       (bool)etl::integral_limits<unsigned int>::is_signed);
      CHECK_EQUAL(std::numeric_limits<long>::is_signed,               (bool)etl::integral_limits<long>::is_signed);
      CHECK_EQUAL(std::numeric_limits<unsigned long>::is_signed,      (bool)etl::integral_limits<unsigned long>::is_signed);
      CHECK_EQUAL(std::numeric_limits<long long>::is_signed,          (bool)etl::integral_limits<long long>::is_signed);
      CHECK_EQUAL(std::numeric_limits<unsigned long long>::is_signed, (bool)etl::integral_limits<unsigned long long>::is_signed);
    }
  };
}
