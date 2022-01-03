/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2021 jwellbelove

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

#include "etl/bit.h"
#include "etl/fnv_1.h"

namespace
{
  // Count bits the long way.
  template <typename T>
  size_t test_count(T value)
  {
    size_t count = 0UL;

    for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
    {
      if ((value & (T(1) << i)) != 0)
      {
        ++count;
      }
    }

    return count;
  }

  // Count trailing zero the long way.
  template <typename T>
  size_t test_trailing_zeros(T value)
  {
    size_t count = 0UL;

    for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
    {
      if ((value & 1) == 0)
      {
        ++count;
      }
      else
      {
        return count;
      }

      value >>= 1;
    }

    return count;
  }

  // Count trailing ones the long way.
  template <typename T>
  size_t test_trailing_ones(T value)
  {
    size_t count = 0UL;

    for (int i = 0; i < etl::integral_limits<T>::bits; ++i)
    {
      if ((value & 1) == 1)
      {
        ++count;
      }

      value >>= 1;
    }

    return count;
  }

  // Check parity the long way.
  template <typename T>
  size_t test_parity(T value)
  {
    size_t count = test_count(value);

    return count & 1;
  }

  // Power of 2.
  uint64_t test_power_of_2(int power)
  {
    uint64_t result = 1ULL;

    for (int i = 0; i < power; ++i)
    {
      result *= 2;
    }

    return result;
  }

  SUITE(test_bit)
  {
    //*************************************************************************
    TEST(test_rotl8)
    {
      uint8_t value;

      value = 0x00U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0, int(value));

      value = 0x21U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x42U, int(value));

      value = 0x42U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x84U, int(value));

      value = 0x84U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x09U, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 2);
      CHECK_EQUAL(0xDEU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 3);
      CHECK_EQUAL(0xBDU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 4);
      CHECK_EQUAL(0x7BU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 5);
      CHECK_EQUAL(0xF6U, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 6);
      CHECK_EQUAL(0xEDU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 7);
      CHECK_EQUAL(0xDBU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 8);
      CHECK_EQUAL(0xB7U, int(value));

      value = 0xB7U;
      value = etl::rotl(value, 9);
      CHECK_EQUAL(0x6FU, int(value));

      value = 0xB7U;
      value = etl::rotl(value, -1);
      CHECK_EQUAL(0xDBU, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_rotl8_constexpr)
    {
      char temp[etl::rotl(uint8_t(0xAAU), 1)];

      CHECK_EQUAL(etl::rotl(uint8_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotl16)
    {
      uint16_t value;

      value = 0x0000U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0, value);

      value = 0x8421U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x0843U, value);

      value = 0x0843U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x1086U, value);

      value = 0x1086U;
      value = etl::rotl(value, 1);
      CHECK_EQUAL(0x210CU, value);

      value = 0xB73CU;
      value = etl::rotl(value, 2);
      CHECK_EQUAL(0xDCF2U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 3);
      CHECK_EQUAL(0xB9E5U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 4);
      CHECK_EQUAL(0x73CBU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 5);
      CHECK_EQUAL(0xE796U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 6);
      CHECK_EQUAL(0xCF2DU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 7);
      CHECK_EQUAL(0x9E5BU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 8);
      CHECK_EQUAL(0x3CB7U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 9);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 9);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 10);
      CHECK_EQUAL(0xF2DCU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 11);
      CHECK_EQUAL(0xE5B9U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 12);
      CHECK_EQUAL(0xCB73U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 13);
      CHECK_EQUAL(0x96E7U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 14);
      CHECK_EQUAL(0x2DCFU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 15);
      CHECK_EQUAL(0x5B9EU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 16);
      CHECK_EQUAL(0xB73CU, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, 17);
      CHECK_EQUAL(0x6E79U, int(value));

      value = 0xB73CU;
      value = etl::rotl(value, -1);
      CHECK_EQUAL(0x5B9EU, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_rotl16_constexpr)
    {
      char temp[etl::rotl(uint16_t(0xAAU), 1)];

      CHECK_EQUAL(etl::rotl(uint16_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotate_right8)
    {
      uint8_t value;

      value = 0x00U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0, int(value));

      value = 0x21U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x90U, int(value));

      value = 0x42U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x21U, int(value));

      value = 0x84U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x42U, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 2);
      CHECK_EQUAL(0xEDU, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 3);
      CHECK_EQUAL(0xF6U, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 4);
      CHECK_EQUAL(0x7BU, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 5);
      CHECK_EQUAL(0xBDU, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 6);
      CHECK_EQUAL(0xDEU, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 7);
      CHECK_EQUAL(0x6FU, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 8);
      CHECK_EQUAL(0xB7U, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, 9);
      CHECK_EQUAL(0xDBU, int(value));

      value = 0xB7U;
      value = etl::rotate_right(value, -1);
      CHECK_EQUAL(0x6FU, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_rotate_right8_constexpr)
    {
      char temp[etl::rotate_right(uint8_t(0xAAU))];

      CHECK_EQUAL(etl::rotate_right(uint8_t(0xAAU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotate_right16)
    {
      uint16_t value;

      value = 0x0000U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0, value);

      value = 0x8421U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0xC210U, value);

      value = 0xC210U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x6108U, value);

      value = 0x6108U;
      value = etl::rotate_right(value);
      CHECK_EQUAL(0x3084U, value);

      value = 0xB73CU;
      value = etl::rotate_right(value, 2);
      CHECK_EQUAL(0x2DCFU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 3);
      CHECK_EQUAL(0x96E7U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 4);
      CHECK_EQUAL(0xCB73U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 5);
      CHECK_EQUAL(0xE5B9U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 6);
      CHECK_EQUAL(0xF2DCU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 7);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 8);
      CHECK_EQUAL(0x3CB7U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 9);
      CHECK_EQUAL(0x9E5BU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 9);
      CHECK_EQUAL(0x9E5BU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 10);
      CHECK_EQUAL(0xCF2DU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 11);
      CHECK_EQUAL(0xE796U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 12);
      CHECK_EQUAL(0x73CBU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 13);
      CHECK_EQUAL(0xB9E5U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 14);
      CHECK_EQUAL(0xDCF2U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 15);
      CHECK_EQUAL(0x6E79U, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 16);
      CHECK_EQUAL(0xB73CU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, 17);
      CHECK_EQUAL(0x5B9EU, int(value));

      value = 0xB73CU;
      value = etl::rotate_right(value, -1);
      CHECK_EQUAL(0x6E79U, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_rotate_right16_constexpr)
    {
      char temp[etl::rotate_right(uint16_t(0xAAU))];

      CHECK_EQUAL(etl::rotate_right(uint16_t(0xAAU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotate16)
    {
      uint16_t value;
      int offset;

      offset = 4;
      value = 0xB73CU;
      value = etl::rotate(value, offset);
      CHECK_EQUAL(0x73CBU, int(value));

      offset = -4;
      value = 0xB73CU;
      value = etl::rotate(value, offset);
      CHECK_EQUAL(0xCB73U, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_rotate16_constexpr)
    {
      char temp[etl::rotate(uint16_t(0xAAU), 1)];

      CHECK_EQUAL(etl::rotate(uint16_t(0xAAU), 1), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bytes16)
    {
      uint16_t value;

      value = 0xFC5AU;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5AFCU, value);

      value = 0x5AA5U;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xA55AU, value);

      value = 0xA55AU;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5AA5U, value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bytes16_constexpr)
    {
      char temp[etl::byteswap(uint16_t(0xA500U))];

      CHECK_EQUAL(etl::byteswap(uint16_t(0xA500U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bytes32)
    {
      uint32_t value;

      value = 0xF0C3A55AUL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5AA5C3F0UL, value);

      value = 0xA5A55A5AUL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5A5AA5A5UL, value);

      value = 0x5A5AA5A5UL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xA5A55A5AUL, value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bytes32_constexpr)
    {
      char temp[etl::byteswap(uint32_t(0xA5000000UL))];

      CHECK_EQUAL(etl::byteswap(uint32_t(0xA5000000UL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bytes64)
    {
      uint64_t value;

      value = 0x0123456789ABCDEFULL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xEFCDAB8967452301ULL, value);

      value = 0xA5A55A5AA5A55A5AULL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0x5A5AA5A55A5AA5A5ULL, value);

      value = 0x5A5AA5A55A5AA5A5ULL;
      value = etl::byteswap(value);
      CHECK_EQUAL(0xA5A55A5AA5A55A5AULL, value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bytes64_constexpr)
    {
      char temp[etl::byteswap(uint64_t(0xA500000000000000ULL))];

      CHECK_EQUAL(etl::byteswap(uint64_t(0xA500000000000000ULL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_bits_8)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::popcount(uint8_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_8_constexpr)
    {
      char temp[etl::popcount(uint8_t(0xFFU))];

      CHECK_EQUAL(etl::popcount(uint8_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_bits_16)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::popcount(uint16_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_16_constexpr)
    {
      char temp[etl::popcount(uint16_t(0xFFU))];

      CHECK_EQUAL(etl::popcount(uint16_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_bits_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0UL; i < 1000000UL; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_count(value), etl::popcount(value));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_32_constexpr)
    {
      char temp[etl::popcount(uint32_t(0xFFU))];

      CHECK_EQUAL(etl::popcount(uint32_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_bits_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0UL; i < 1000000UL; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_count(value), etl::popcount(value));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_64_constexpr)
    {
      char temp[etl::popcount(uint64_t(0xFFU))];

      CHECK_EQUAL(etl::popcount(uint64_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_zeros_8)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_8_constexpr)
    {
      char temp[etl::countr_zero(uint8_t(0x08))];

      CHECK_EQUAL(etl::countr_zero(uint8_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_zeros_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_16_constexpr)
    {
      char temp[etl::countr_zero(uint16_t(0x08))];

      CHECK_EQUAL(etl::countr_zero(uint16_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_zeros_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_32_constexpr)
    {
      char temp[etl::countr_zero(uint32_t(0x08))];

      CHECK_EQUAL(etl::countr_zero(uint32_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_zeros_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_trailing_zeros(value), etl::countr_zero(value));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_64_constexpr)
    {
      char temp[etl::countr_zero(uint64_t(0x08))];

      CHECK_EQUAL(etl::countr_zero(uint64_t(0x08)), sizeof(temp));
    }
#endif
//    //*************************************************************************
//    TEST(test_max_value_for_bits)
//    {
//        // Check that the values are correct.
//        //CHECK_EQUAL(0U, etl::max_value_for_nbits<0>::value);
//        CHECK_EQUAL(1U, etl::max_value_for_nbits<1>::value);
//        CHECK_EQUAL(3U, etl::max_value_for_nbits<2>::value);
//        CHECK_EQUAL(7U, etl::max_value_for_nbits<3>::value);
//        CHECK_EQUAL(15U, etl::max_value_for_nbits<4>::value);
//        CHECK_EQUAL(31U, etl::max_value_for_nbits<5>::value);
//        CHECK_EQUAL(63U, etl::max_value_for_nbits<6>::value);
//        CHECK_EQUAL(127U, etl::max_value_for_nbits<7>::value);
//        CHECK_EQUAL(255U, etl::max_value_for_nbits<8>::value);
//        CHECK_EQUAL(511U, etl::max_value_for_nbits<9>::value);
//        CHECK_EQUAL(1023U, etl::max_value_for_nbits<10>::value);
//        CHECK_EQUAL(2047U, etl::max_value_for_nbits<11>::value);
//        CHECK_EQUAL(4095U, etl::max_value_for_nbits<12>::value);
//        CHECK_EQUAL(8191U, etl::max_value_for_nbits<13>::value);
//        CHECK_EQUAL(16383U, etl::max_value_for_nbits<14>::value);
//        CHECK_EQUAL(32767U, etl::max_value_for_nbits<15>::value);
//        CHECK_EQUAL(65535U, etl::max_value_for_nbits<16>::value);
//        CHECK_EQUAL(131071UL, etl::max_value_for_nbits<17>::value);
//        CHECK_EQUAL(262143UL, etl::max_value_for_nbits<18>::value);
//        CHECK_EQUAL(524287UL, etl::max_value_for_nbits<19>::value);
//        CHECK_EQUAL(1048575UL, etl::max_value_for_nbits<20>::value);
//        CHECK_EQUAL(2097151UL, etl::max_value_for_nbits<21>::value);
//        CHECK_EQUAL(4194303UL, etl::max_value_for_nbits<22>::value);
//        CHECK_EQUAL(8388607UL, etl::max_value_for_nbits<23>::value);
//        CHECK_EQUAL(16777215UL, etl::max_value_for_nbits<24>::value);
//        CHECK_EQUAL(33554431UL, etl::max_value_for_nbits<25>::value);
//        CHECK_EQUAL(67108863UL, etl::max_value_for_nbits<26>::value);
//        CHECK_EQUAL(134217727UL, etl::max_value_for_nbits<27>::value);
//        CHECK_EQUAL(268435455UL, etl::max_value_for_nbits<28>::value);
//        CHECK_EQUAL(536870911UL, etl::max_value_for_nbits<29>::value);
//        CHECK_EQUAL(1073741823UL, etl::max_value_for_nbits<30>::value);
//        CHECK_EQUAL(2147483647UL, etl::max_value_for_nbits<31>::value);
//        CHECK_EQUAL(4294967295UL, etl::max_value_for_nbits<32>::value);
//        CHECK_EQUAL(8589934591ULL, etl::max_value_for_nbits<33>::value);
//        CHECK_EQUAL(17179869183ULL, etl::max_value_for_nbits<34>::value);
//        CHECK_EQUAL(34359738367ULL, etl::max_value_for_nbits<35>::value);
//        CHECK_EQUAL(68719476735ULL, etl::max_value_for_nbits<36>::value);
//        CHECK_EQUAL(137438953471ULL, etl::max_value_for_nbits<37>::value);
//        CHECK_EQUAL(274877906943ULL, etl::max_value_for_nbits<38>::value);
//        CHECK_EQUAL(549755813887ULL, etl::max_value_for_nbits<39>::value);
//        CHECK_EQUAL(1099511627775ULL, etl::max_value_for_nbits<40>::value);
//        CHECK_EQUAL(2199023255551ULL, etl::max_value_for_nbits<41>::value);
//        CHECK_EQUAL(4398046511103ULL, etl::max_value_for_nbits<42>::value);
//        CHECK_EQUAL(8796093022207ULL, etl::max_value_for_nbits<43>::value);
//        CHECK_EQUAL(17592186044415ULL, etl::max_value_for_nbits<44>::value);
//        CHECK_EQUAL(35184372088831ULL, etl::max_value_for_nbits<45>::value);
//        CHECK_EQUAL(70368744177663ULL, etl::max_value_for_nbits<46>::value);
//        CHECK_EQUAL(140737488355327ULL, etl::max_value_for_nbits<47>::value);
//        CHECK_EQUAL(281474976710655ULL, etl::max_value_for_nbits<48>::value);
//        CHECK_EQUAL(562949953421311ULL, etl::max_value_for_nbits<49>::value);
//        CHECK_EQUAL(1125899906842623ULL, etl::max_value_for_nbits<50>::value);
//        CHECK_EQUAL(2251799813685247ULL, etl::max_value_for_nbits<51>::value);
//        CHECK_EQUAL(4503599627370495ULL, etl::max_value_for_nbits<52>::value);
//        CHECK_EQUAL(9007199254740991ULL, etl::max_value_for_nbits<53>::value);
//        CHECK_EQUAL(18014398509481983ULL, etl::max_value_for_nbits<54>::value);
//        CHECK_EQUAL(36028797018963967ULL, etl::max_value_for_nbits<55>::value);
//        CHECK_EQUAL(72057594037927935ULL, etl::max_value_for_nbits<56>::value);
//        CHECK_EQUAL(144115188075855871ULL, etl::max_value_for_nbits<57>::value);
//        CHECK_EQUAL(288230376151711743ULL, etl::max_value_for_nbits<58>::value);
//        CHECK_EQUAL(576460752303423487ULL, etl::max_value_for_nbits<59>::value);
//        CHECK_EQUAL(1152921504606846975ULL, etl::max_value_for_nbits<60>::value);
//        CHECK_EQUAL(2305843009213693951ULL, etl::max_value_for_nbits<61>::value);
//        CHECK_EQUAL(4611686018427387903ULL, etl::max_value_for_nbits<62>::value);
//        CHECK_EQUAL(9223372036854775807ULL, etl::max_value_for_nbits<63>::value);
//        CHECK_EQUAL(18446744073709551615ULL, etl::max_value_for_nbits<64>::value);
//
//        // Check that the value types are correct.
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<0>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<1>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<2>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<3>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<4>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<5>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<6>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<7>::value_type>::value));
//        CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<8>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<9>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<10>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<11>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<12>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<13>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<14>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<15>::value_type>::value));
//        CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<16>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<17>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<18>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<19>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<20>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<21>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<22>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<23>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<24>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<25>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<26>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<27>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<28>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<29>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<30>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<31>::value_type>::value));
//        CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<32>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<33>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<34>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<35>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<36>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<37>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<38>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<39>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<40>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<41>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<42>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<43>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<44>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<45>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<46>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<47>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<48>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<49>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<50>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<51>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<52>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<53>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<54>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<55>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<56>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<57>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<58>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<59>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<60>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<61>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<62>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<63>::value_type>::value));
//        CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<64>::value_type>::value));
//    }
  };
}

