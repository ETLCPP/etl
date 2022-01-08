/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
https://www.etlcpp.com

Copyright(c) 2014 jwellbelove

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

#include <cstdint>
#include <type_traits>
#include <limits>

#include "etl/binary.h"
#include "etl/bitset.h"
#include "etl/fnv_1.h"
#include "etl/integral_limits.h"
#include "etl/type_traits.h"

namespace
{
  //***********************************
  // Count bits the easy way.
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

  //***********************************
  // Count trailing zeros the long way.
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

  //***********************************
  // Count leading zeros the long way.
  template <typename T>
  size_t test_leading_zeros(T value)
  {
    value = etl::reverse_bits(value);
    return test_trailing_zeros(value);
  }

  //***********************************
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
      else
      {
        return count;
      }

      value >>= 1;
    }

    return count;
  }

  //***********************************
  // Count leading ones the long way.
  template <typename T>
  size_t test_leading_ones(T value)
  {
    value = etl::reverse_bits(value);
    return test_trailing_ones(value);
  }

  //***********************************
  // Check parity the easy way.
  template <typename T>
  size_t test_parity(T value)
  {
    size_t count = test_count(value);

    return count & 1;
  }

  //***********************************
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

  //***********************************
  // Fold bits.
  template <typename TReturn>
  TReturn test_fold_bits(uint64_t value, int size)
  {
    int bits_remaining = 64;
    uint64_t mask = test_power_of_2(size) - 1ULL;
    TReturn  result = 0;

    while (bits_remaining > size)
    {
      result = result ^ (value & mask);
      value = value >> size;
      bits_remaining -= size;
    }

    result = result ^ (value & mask);

    return result;
  }

  //***********************************
  // Slow gray to binary
  template <typename T>
  T compare_gray_to_binary(T value_)
  {
    typedef typename std::make_unsigned<T>::type type;

    type value = type(value_);

    T mask;
    for (mask = value >> 1U; mask != 0; mask = mask >> 1U)
    {
      value = value ^ mask;
    }

    return value;
  }

  SUITE(test_binary)
  {
    //*************************************************************************
    TEST(test_rotate_left8)
    {
      uint8_t value;

      value = 0x00U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0, int(value));

      value = 0x21U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x42U, int(value));

      value = 0x42U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x84U, int(value));

      value = 0x84U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x09U, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 2);
      CHECK_EQUAL(0xDEU, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 3);
      CHECK_EQUAL(0xBDU, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 4);
      CHECK_EQUAL(0x7BU, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 5);
      CHECK_EQUAL(0xF6U, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 6);
      CHECK_EQUAL(0xEDU, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 7);
      CHECK_EQUAL(0xDBU, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 8);
      CHECK_EQUAL(0xB7U, int(value));

      value = 0xB7U;
      value = etl::rotate_left(value, 9);
      CHECK_EQUAL(0x6FU, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_rotate_left8_constexpr)
    {
      char temp[etl::rotate_left(uint8_t(0xAAU))];

      CHECK_EQUAL(etl::rotate_left(uint8_t(0xAAU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_rotate_left16)
    {
      uint16_t value;

      value = 0x0000U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0, value);

      value = 0x8421U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x0843U, value);

      value = 0x0843U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x1086U, value);

      value = 0x1086U;
      value = etl::rotate_left(value);
      CHECK_EQUAL(0x210CU, value);

      value = 0xB73CU;
      value = etl::rotate_left(value, 2);
      CHECK_EQUAL(0xDCF2U, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 3);
      CHECK_EQUAL(0xB9E5U, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 4);
      CHECK_EQUAL(0x73CBU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 5);
      CHECK_EQUAL(0xE796U, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 6);
      CHECK_EQUAL(0xCF2DU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 7);
      CHECK_EQUAL(0x9E5BU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 8);
      CHECK_EQUAL(0x3CB7U, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 9);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 9);
      CHECK_EQUAL(0x796EU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 10);
      CHECK_EQUAL(0xF2DCU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 11);
      CHECK_EQUAL(0xE5B9U, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 12);
      CHECK_EQUAL(0xCB73U, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 13);
      CHECK_EQUAL(0x96E7U, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 14);
      CHECK_EQUAL(0x2DCFU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 15);
      CHECK_EQUAL(0x5B9EU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 16);
      CHECK_EQUAL(0xB73CU, int(value));

      value = 0xB73CU;
      value = etl::rotate_left(value, 17);
      CHECK_EQUAL(0x6E79U, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_rotate_left16_constexpr)
    {
      char temp[etl::rotate_left(uint16_t(0xAAU))];

      CHECK_EQUAL(etl::rotate_left(uint16_t(0xAAU)), sizeof(temp));
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
    TEST(test_reverse_bits8)
    {
      uint8_t value;

      value = 0xF0U;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x0FU, int(value));

      value = 0x0FU;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0xF0U, int(value));

      value = 0xA5U;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0xA5U, int(value));

      value = 0x5AU;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x5AU, int(value));

      value = 0xCAU;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x53U, int(value));

      value = 0xACU;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x35U, int(value));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bits8_constexpr)
    {
      char temp[etl::reverse_bits(uint8_t(0xA5U))];

      CHECK_EQUAL(etl::reverse_bits(uint8_t(0xA5U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bits16)
    {
      uint16_t value;

      value = 0xFC5AU;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x5A3FU, value);

      value = 0x5A5AU;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0x5A5AU, value);

      value = 0xA5A5U;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(0xA5A5U, value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bits16_constexpr)
    {
      char temp[etl::reverse_bits(uint16_t(0xA500U))];

      CHECK_EQUAL(etl::reverse_bits(uint16_t(0xA500U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bits32)
    {
      uint32_t value;

      value = 0xF0C3A55AUL;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint32_t(0x5AA5C30FUL), value);

      value = 0xA5A5A5A5UL;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint32_t(0xA5A5A5A5UL), value);

      value = 0x5A5A5A5AUL;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint32_t(0x5A5A5A5AUL), value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bits32_constexpr)
    {
      char temp[etl::reverse_bits(uint32_t(0xA5000000UL))];

      CHECK_EQUAL(etl::reverse_bits(uint32_t(0xA5000000UL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bits64)
    {
      uint64_t value;

      value = 0x1234ABCDF0C3A55AULL;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint64_t(0x5AA5C30FB3D52C48ULL), value);

      value = 0xA5A5A5A5A5A5A5A5ULL;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint64_t(0xA5A5A5A5A5A5A5A5ULL), value);

      value = 0x5A5A5A5A5A5A5A5AULL;
      value = etl::reverse_bits(value);
      CHECK_EQUAL(uint64_t(0x5A5A5A5A5A5A5A5AULL), value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bits64_constexpr)
    {
      char temp[etl::reverse_bits(uint64_t(0xA500000000000000ULL))];

      CHECK_EQUAL(etl::reverse_bits(uint64_t(0xA500000000000000ULL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bytes16)
    {
      uint16_t value;

      value = 0xFC5AU;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5AFCU, value);

      value = 0x5AA5U;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xA55AU, value);

      value = 0xA55AU;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5AA5U, value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bytes16_constexpr)
    {
      char temp[etl::reverse_bytes(uint16_t(0xA500U))];

      CHECK_EQUAL(etl::reverse_bytes(uint16_t(0xA500U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bytes32)
    {
      uint32_t value;

      value = 0xF0C3A55AUL;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5AA5C3F0UL, value);

      value = 0xA5A55A5AUL;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5A5AA5A5UL, value);

      value = 0x5A5AA5A5UL;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xA5A55A5AUL, value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bytes32_constexpr)
    {
      char temp[etl::reverse_bytes(uint32_t(0xA5000000UL))];

      CHECK_EQUAL(etl::reverse_bytes(uint32_t(0xA5000000UL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_reverse_bytes64)
    {
      uint64_t value;

      value = 0x0123456789ABCDEFULL;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xEFCDAB8967452301ULL, value);

      value = 0xA5A55A5AA5A55A5AULL;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0x5A5AA5A55A5AA5A5ULL, value);

      value = 0x5A5AA5A55A5AA5A5ULL;
      value = etl::reverse_bytes(value);
      CHECK_EQUAL(0xA5A55A5AA5A55A5AULL, value);
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_reverse_bytes64_constexpr)
    {
      char temp[etl::reverse_bytes(uint64_t(0xA500000000000000ULL))];

      CHECK_EQUAL(etl::reverse_bytes(uint64_t(0xA500000000000000ULL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_binary_to_gray8)
    {
      uint8_t last_gray = 0U;

      for (size_t i = 1UL; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        uint8_t gray = etl::binary_to_gray(uint8_t(i));
        uint8_t result = gray ^ last_gray;
        last_gray = gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        // Only one bit should be set.
        CHECK(pass);
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_binary_to_gray8_constexpr)
    {
      char temp[etl::binary_to_gray(uint8_t(0xFFU))];

      CHECK_EQUAL(etl::binary_to_gray(uint8_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_binary_to_gray16)
    {
      uint16_t last_gray = 0U;

      for (size_t i = 1UL; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        uint16_t gray   = etl::binary_to_gray(uint16_t(i));
        uint16_t result = gray ^ last_gray;
        last_gray       = gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        // Only one bit should be set.
        CHECK(pass);
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_binary_to_gray16_constexpr)
    {
      char temp[etl::binary_to_gray(uint16_t(0xFFU))];

      CHECK_EQUAL(etl::binary_to_gray(uint16_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_binary_to_gray32)
    {
      etl::fnv_1_32 hash;

      hash.add(1);

      for (size_t i = 1UL; i < 1000000UL; ++i)
      {
        uint32_t value = hash.value();

        uint32_t last_gray = etl::binary_to_gray(uint32_t(value));
        uint32_t gray      = etl::binary_to_gray(uint32_t(value + 1));
        uint32_t result    = gray ^ last_gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        hash.add(1);

        // Only one bit should be set.
        CHECK(pass);
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_binary_to_gray32_constexpr)
    {
      char temp[etl::binary_to_gray(uint32_t(0xFFU))];

      CHECK_EQUAL(etl::binary_to_gray(uint32_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_binary_to_gray64)
    {
      etl::fnv_1a_64 hash;

      hash.add(1);

      for (size_t i = 1UL; i < 1000000UL; ++i)
      {
        uint64_t value = hash.value();

        uint64_t last_gray = etl::binary_to_gray(uint64_t(value));
        uint64_t gray = etl::binary_to_gray(uint64_t(value + 1));
        uint64_t result = gray ^ last_gray;

        bool pass = ((result != 0) && !(result & (result - 1)));

        hash.add(1);

        // Only one bit should be set.
        CHECK(pass);
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_binary_to_gray64_constexpr)
    {
      char temp[etl::binary_to_gray(uint64_t(0xFFU))];

      CHECK_EQUAL(etl::binary_to_gray(uint64_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_gray_to_binary8)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(compare_gray_to_binary(uint8_t(i)), etl::gray_to_binary(uint8_t(i)));
        CHECK_EQUAL(compare_gray_to_binary(int8_t(i)), etl::gray_to_binary(int8_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_gray_to_binary8_constexpr)
    {
      char temp[etl::gray_to_binary(uint8_t(0xFFU))];

      CHECK_EQUAL(etl::gray_to_binary(uint8_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_gray_to_binary16)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(compare_gray_to_binary(uint16_t(i)), etl::gray_to_binary(uint16_t(i)));
        CHECK_EQUAL(compare_gray_to_binary(int16_t(i)), etl::gray_to_binary(int16_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_gray_to_binary16_constexpr)
    {
      char temp[etl::gray_to_binary(uint16_t(0xFFU))];

      CHECK_EQUAL(etl::gray_to_binary(uint16_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_gray_to_binary32)
    {
      etl::fnv_1a_32 hash;

      hash.add(1);

      for (size_t i = 1UL; i < 1000000UL; ++i)
      {
        uint32_t value = hash.value();
        CHECK_EQUAL(compare_gray_to_binary(value), etl::gray_to_binary(value));
        CHECK_EQUAL(int32_t(compare_gray_to_binary(value)), etl::gray_to_binary(int32_t(value)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_gray_to_binary32_constexpr)
    {
      char temp[etl::gray_to_binary(uint32_t(0xFFU))];

      CHECK_EQUAL(etl::gray_to_binary(uint32_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_gray_to_binary64)
    {
      etl::fnv_1a_64 hash;

      hash.add(1);

      for (size_t i = 1UL; i < 1000000UL; ++i)
      {
        uint64_t value = hash.value();
        CHECK_EQUAL(compare_gray_to_binary(value), etl::gray_to_binary(value));
        CHECK_EQUAL(int64_t(compare_gray_to_binary(value)), etl::gray_to_binary(int64_t(value)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_gray_to_binary64_constexpr)
    {
      char temp[etl::gray_to_binary(uint64_t(0xFFU))];

      CHECK_EQUAL(etl::gray_to_binary(uint64_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_bits_8)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::count_bits(uint8_t(i)));
        CHECK_EQUAL(test_count(i), etl::count_bits(int8_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_8_constexpr)
    {
      char temp[etl::count_bits(uint8_t(0xFFU))];

      CHECK_EQUAL(etl::count_bits(uint8_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_bits_16)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(test_count(i), etl::count_bits(uint16_t(i)));
        CHECK_EQUAL(test_count(i), etl::count_bits(int16_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_16_constexpr)
    {
      char temp[etl::count_bits(uint16_t(0xFFU))];

      CHECK_EQUAL(etl::count_bits(uint16_t(0xFFU)), sizeof(temp));
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

        CHECK_EQUAL(test_count(value), etl::count_bits(value));
        CHECK_EQUAL(test_count(value), etl::count_bits(int32_t(value)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_32_constexpr)
    {
      char temp[etl::count_bits(uint32_t(0xFFU))];

      CHECK_EQUAL(etl::count_bits(uint32_t(0xFFU)), sizeof(temp));
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

        CHECK_EQUAL(test_count(value), etl::count_bits(value));
        CHECK_EQUAL(test_count(value), etl::count_bits(int64_t(value)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_bits_64_constexpr)
    {
      char temp[etl::count_bits(uint64_t(0xFFU))];

      CHECK_EQUAL(etl::count_bits(uint64_t(0xFFU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_parity_8)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint8_t>::max(); ++i)
      {
        CHECK_EQUAL(test_parity(i), etl::parity(uint8_t(i)));
        CHECK_EQUAL(test_parity(i), etl::parity(int8_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_parity_8_constexpr)
    {
      char temp[etl::parity(uint8_t(0xFEU))];

      CHECK_EQUAL(etl::parity(uint8_t(0xFEU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_parity_16)
    {
      for (size_t i = 1UL; i <= std::numeric_limits<uint16_t>::max(); ++i)
      {
        CHECK_EQUAL(test_parity(i), etl::parity(uint16_t(i)));
        CHECK_EQUAL(test_parity(i), etl::parity(int16_t(i)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_parity_16_constexpr)
    {
      char temp[etl::parity(uint16_t(0xFEU))];

      CHECK_EQUAL(etl::parity(uint16_t(0xFEU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_parity_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0UL; i < 1000000UL; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_parity(value), etl::parity(value));
        CHECK_EQUAL(test_parity(value), etl::parity(int32_t(value)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_parity_32_constexpr)
    {
      char temp[etl::parity(uint32_t(0xFEU))];

      CHECK_EQUAL(etl::parity(uint32_t(0xFEU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_parity_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0UL; i < 1000000UL; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_parity(value), etl::parity(value));
        CHECK_EQUAL(test_parity(value), etl::parity(int64_t(value)));
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_parity_64_constexpr)
    {
      char temp[etl::parity(uint64_t(0xFEU))];

      CHECK_EQUAL(etl::parity(uint64_t(0xFEU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_fold_bits)
    {
      const uint64_t data = 0xE8C9AACCBC3D9A8FULL;

      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  1), (etl::fold_bits<uint64_t,  1>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  2), (etl::fold_bits<uint64_t,  2>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  3), (etl::fold_bits<uint64_t,  3>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  4), (etl::fold_bits<uint64_t,  4>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  5), (etl::fold_bits<uint64_t,  5>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  6), (etl::fold_bits<uint64_t,  6>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  7), (etl::fold_bits<uint64_t,  7>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  8), (etl::fold_bits<uint64_t,  8>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data,  9), (etl::fold_bits<uint64_t,  9>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 10), (etl::fold_bits<uint64_t, 10>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 11), (etl::fold_bits<uint64_t, 11>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 12), (etl::fold_bits<uint64_t, 12>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 13), (etl::fold_bits<uint64_t, 13>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 14), (etl::fold_bits<uint64_t, 14>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 15), (etl::fold_bits<uint64_t, 15>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 16), (etl::fold_bits<uint64_t, 16>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 17), (etl::fold_bits<uint64_t, 17>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 18), (etl::fold_bits<uint64_t, 18>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 19), (etl::fold_bits<uint64_t, 19>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 20), (etl::fold_bits<uint64_t, 20>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 21), (etl::fold_bits<uint64_t, 21>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 22), (etl::fold_bits<uint64_t, 22>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 23), (etl::fold_bits<uint64_t, 23>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 24), (etl::fold_bits<uint64_t, 24>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 25), (etl::fold_bits<uint64_t, 25>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 26), (etl::fold_bits<uint64_t, 26>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 27), (etl::fold_bits<uint64_t, 27>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 28), (etl::fold_bits<uint64_t, 28>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 29), (etl::fold_bits<uint64_t, 29>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 30), (etl::fold_bits<uint64_t, 30>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 31), (etl::fold_bits<uint64_t, 31>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 32), (etl::fold_bits<uint64_t, 32>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 33), (etl::fold_bits<uint64_t, 33>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 34), (etl::fold_bits<uint64_t, 34>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 35), (etl::fold_bits<uint64_t, 35>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 36), (etl::fold_bits<uint64_t, 36>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 37), (etl::fold_bits<uint64_t, 37>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 38), (etl::fold_bits<uint64_t, 38>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 39), (etl::fold_bits<uint64_t, 39>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 40), (etl::fold_bits<uint64_t, 40>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 41), (etl::fold_bits<uint64_t, 41>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 42), (etl::fold_bits<uint64_t, 42>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 43), (etl::fold_bits<uint64_t, 43>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 44), (etl::fold_bits<uint64_t, 44>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 45), (etl::fold_bits<uint64_t, 45>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 46), (etl::fold_bits<uint64_t, 46>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 47), (etl::fold_bits<uint64_t, 47>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 48), (etl::fold_bits<uint64_t, 48>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 49), (etl::fold_bits<uint64_t, 49>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 50), (etl::fold_bits<uint64_t, 50>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 51), (etl::fold_bits<uint64_t, 51>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 52), (etl::fold_bits<uint64_t, 52>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 53), (etl::fold_bits<uint64_t, 53>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 54), (etl::fold_bits<uint64_t, 54>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 55), (etl::fold_bits<uint64_t, 55>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 56), (etl::fold_bits<uint64_t, 56>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 57), (etl::fold_bits<uint64_t, 57>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 58), (etl::fold_bits<uint64_t, 58>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 59), (etl::fold_bits<uint64_t, 59>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 60), (etl::fold_bits<uint64_t, 60>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 61), (etl::fold_bits<uint64_t, 61>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 62), (etl::fold_bits<uint64_t, 62>(data)));
      CHECK_EQUAL(test_fold_bits<uint64_t>(data, 63), (etl::fold_bits<uint64_t, 63>(data)));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_fold_bits_constexpr)
    {
      char temp[etl::fold_bits<int64_t, 7>(0xE8C9AACCBC3D9A8FULL)];

      CHECK_EQUAL((etl::fold_bits<int64_t, 7>(0xE8C9AACCBC3D9A8FULL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_max_value_for_bits)
    {
      // Check that the values are correct.
      //CHECK_EQUAL(0U, etl::max_value_for_nbits<0>::value);
      CHECK_EQUAL(1U, etl::max_value_for_nbits<1>::value);
      CHECK_EQUAL(3U, etl::max_value_for_nbits<2>::value);
      CHECK_EQUAL(7U, etl::max_value_for_nbits<3>::value);
      CHECK_EQUAL(15U, etl::max_value_for_nbits<4>::value);
      CHECK_EQUAL(31U, etl::max_value_for_nbits<5>::value);
      CHECK_EQUAL(63U, etl::max_value_for_nbits<6>::value);
      CHECK_EQUAL(127U, etl::max_value_for_nbits<7>::value);
      CHECK_EQUAL(255U, etl::max_value_for_nbits<8>::value);
      CHECK_EQUAL(511U, etl::max_value_for_nbits<9>::value);
      CHECK_EQUAL(1023U, etl::max_value_for_nbits<10>::value);
      CHECK_EQUAL(2047U, etl::max_value_for_nbits<11>::value);
      CHECK_EQUAL(4095U, etl::max_value_for_nbits<12>::value);
      CHECK_EQUAL(8191U, etl::max_value_for_nbits<13>::value);
      CHECK_EQUAL(16383U, etl::max_value_for_nbits<14>::value);
      CHECK_EQUAL(32767U, etl::max_value_for_nbits<15>::value);
      CHECK_EQUAL(65535U, etl::max_value_for_nbits<16>::value);
      CHECK_EQUAL(131071UL, etl::max_value_for_nbits<17>::value);
      CHECK_EQUAL(262143UL, etl::max_value_for_nbits<18>::value);
      CHECK_EQUAL(524287UL, etl::max_value_for_nbits<19>::value);
      CHECK_EQUAL(1048575UL, etl::max_value_for_nbits<20>::value);
      CHECK_EQUAL(2097151UL, etl::max_value_for_nbits<21>::value);
      CHECK_EQUAL(4194303UL, etl::max_value_for_nbits<22>::value);
      CHECK_EQUAL(8388607UL, etl::max_value_for_nbits<23>::value);
      CHECK_EQUAL(16777215UL, etl::max_value_for_nbits<24>::value);
      CHECK_EQUAL(33554431UL, etl::max_value_for_nbits<25>::value);
      CHECK_EQUAL(67108863UL, etl::max_value_for_nbits<26>::value);
      CHECK_EQUAL(134217727UL, etl::max_value_for_nbits<27>::value);
      CHECK_EQUAL(268435455UL, etl::max_value_for_nbits<28>::value);
      CHECK_EQUAL(536870911UL, etl::max_value_for_nbits<29>::value);
      CHECK_EQUAL(1073741823UL, etl::max_value_for_nbits<30>::value);
      CHECK_EQUAL(2147483647UL, etl::max_value_for_nbits<31>::value);
      CHECK_EQUAL(4294967295UL, etl::max_value_for_nbits<32>::value);
      CHECK_EQUAL(8589934591ULL, etl::max_value_for_nbits<33>::value);
      CHECK_EQUAL(17179869183ULL, etl::max_value_for_nbits<34>::value);
      CHECK_EQUAL(34359738367ULL, etl::max_value_for_nbits<35>::value);
      CHECK_EQUAL(68719476735ULL, etl::max_value_for_nbits<36>::value);
      CHECK_EQUAL(137438953471ULL, etl::max_value_for_nbits<37>::value);
      CHECK_EQUAL(274877906943ULL, etl::max_value_for_nbits<38>::value);
      CHECK_EQUAL(549755813887ULL, etl::max_value_for_nbits<39>::value);
      CHECK_EQUAL(1099511627775ULL, etl::max_value_for_nbits<40>::value);
      CHECK_EQUAL(2199023255551ULL, etl::max_value_for_nbits<41>::value);
      CHECK_EQUAL(4398046511103ULL, etl::max_value_for_nbits<42>::value);
      CHECK_EQUAL(8796093022207ULL, etl::max_value_for_nbits<43>::value);
      CHECK_EQUAL(17592186044415ULL, etl::max_value_for_nbits<44>::value);
      CHECK_EQUAL(35184372088831ULL, etl::max_value_for_nbits<45>::value);
      CHECK_EQUAL(70368744177663ULL, etl::max_value_for_nbits<46>::value);
      CHECK_EQUAL(140737488355327ULL, etl::max_value_for_nbits<47>::value);
      CHECK_EQUAL(281474976710655ULL, etl::max_value_for_nbits<48>::value);
      CHECK_EQUAL(562949953421311ULL, etl::max_value_for_nbits<49>::value);
      CHECK_EQUAL(1125899906842623ULL, etl::max_value_for_nbits<50>::value);
      CHECK_EQUAL(2251799813685247ULL, etl::max_value_for_nbits<51>::value);
      CHECK_EQUAL(4503599627370495ULL, etl::max_value_for_nbits<52>::value);
      CHECK_EQUAL(9007199254740991ULL, etl::max_value_for_nbits<53>::value);
      CHECK_EQUAL(18014398509481983ULL, etl::max_value_for_nbits<54>::value);
      CHECK_EQUAL(36028797018963967ULL, etl::max_value_for_nbits<55>::value);
      CHECK_EQUAL(72057594037927935ULL, etl::max_value_for_nbits<56>::value);
      CHECK_EQUAL(144115188075855871ULL, etl::max_value_for_nbits<57>::value);
      CHECK_EQUAL(288230376151711743ULL, etl::max_value_for_nbits<58>::value);
      CHECK_EQUAL(576460752303423487ULL, etl::max_value_for_nbits<59>::value);
      CHECK_EQUAL(1152921504606846975ULL, etl::max_value_for_nbits<60>::value);
      CHECK_EQUAL(2305843009213693951ULL, etl::max_value_for_nbits<61>::value);
      CHECK_EQUAL(4611686018427387903ULL, etl::max_value_for_nbits<62>::value);
      CHECK_EQUAL(9223372036854775807ULL, etl::max_value_for_nbits<63>::value);
      CHECK_EQUAL(18446744073709551615ULL, etl::max_value_for_nbits<64>::value);
    }

    //*************************************************************************
    TEST(test_max_value_for_bits_types)
    {
      // Check that the value types are correct.
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<0>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<1>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<2>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<3>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<4>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<5>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<6>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<7>::value_type>::value));
      CHECK((etl::is_same<uint8_t,  etl::max_value_for_nbits<8>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<9>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<10>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<11>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<12>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<13>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<14>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<15>::value_type>::value));
      CHECK((etl::is_same<uint16_t, etl::max_value_for_nbits<16>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<17>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<18>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<19>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<20>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<21>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<22>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<23>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<24>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<25>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<26>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<27>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<28>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<29>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<30>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<31>::value_type>::value));
      CHECK((etl::is_same<uint32_t, etl::max_value_for_nbits<32>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<33>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<34>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<35>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<36>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<37>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<38>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<39>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<40>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<41>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<42>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<43>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<44>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<45>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<46>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<47>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<48>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<49>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<50>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<51>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<52>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<53>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<54>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<55>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<56>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<57>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<58>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<59>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<60>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<61>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<62>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<63>::value_type>::value));
      CHECK((etl::is_same<uint64_t, etl::max_value_for_nbits<64>::value_type>::value));
    }

    //*************************************************************************
    TEST(test_sign_extend_template1)
    {
      uint8_t value8 = 0x2AU;
      value8 &= 0x3FU; // 6 bit number.

      CHECK_EQUAL(-22, (etl::sign_extend<int8_t,  6>(value8)));
      CHECK_EQUAL(-22, (etl::sign_extend<int16_t, 6>(value8)));
      CHECK_EQUAL(-22, (etl::sign_extend<int32_t, 6>(value8)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t, 6>(value8)));

      uint16_t value16 = 0x2AAAU;
      value16 &= 0x3FFFU; // 14 bit number.

      CHECK_EQUAL(-5462, (etl::sign_extend<int16_t, 14>(value16)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int32_t, 14>(value16)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int64_t, 14>(value16)));

      uint32_t value32 = 0x2AAAAAAAUL;
      value32 &= 0x3FFFFFFFUL; // 30 bit number.

      CHECK_EQUAL(-357913942, (etl::sign_extend<int32_t, 30>(value32)));
      CHECK_EQUAL(-357913942, (etl::sign_extend<int64_t, 30>(value32)));

      value8 = 0x0AU;
      value8 &= 0x3FU; // 6 bit number.

      CHECK_EQUAL(10, (etl::sign_extend<int8_t, 6>(value8)));
      CHECK_EQUAL(10, (etl::sign_extend<int16_t, 6>(value8)));
      CHECK_EQUAL(10, (etl::sign_extend<int32_t, 6>(value8)));
      CHECK_EQUAL(10, (etl::sign_extend<int64_t, 6>(value8)));

      value16 = 0x0AAAU;
      value16 &= 0x3FFFU; // 14 bit number.

      CHECK_EQUAL(2730, (etl::sign_extend<int16_t, 14>(value16)));
      CHECK_EQUAL(2730, (etl::sign_extend<int32_t, 14>(value16)));
      CHECK_EQUAL(2730, (etl::sign_extend<int64_t, 14>(value16)));

      value32 = 0x0AAAAAAAUL;
      value32 &= 0x3FFFFFFFUL; // 30 bit number.

      CHECK_EQUAL(178956970, (etl::sign_extend<int32_t, 30>(value32)));
      CHECK_EQUAL(178956970, (etl::sign_extend<int64_t, 30>(value32)));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_sign_extend_template1_constexpr)
    {
      char temp[etl::sign_extend<int8_t, 6>(0x1AU)];

      CHECK_EQUAL((etl::sign_extend<int8_t, 6>(0x1AU)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_sign_extend_template1b)
    {
      uint8_t value8     = 0x2AU;
      uint8_t value8mask = 0x3FU;

      const uint32_t value_initial = 0x55555555UL;

      uint32_t value;

      // Shift 0
      value = value_initial;
      value &= ~value8mask;
      value |= value8;

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t, 6, 0>(value)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t, 6, 0>(value)));

      // Shift 3
      value = value_initial;
      value &= ~(value8mask << 3);
      value |= (value8 << 3);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t, 6, 3>(value)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t, 6, 3>(value)));

      // Shift 6
      value = value_initial;
      value &= ~(value8mask << 6);
      value |= (value8 << 6);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t, 6, 6>(value)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t, 6, 6>(value)));

      // Shift 12
      value = value_initial;
      value &= ~(value8mask << 12);
      value |= (value8 << 12);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t, 6, 12>(value)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t, 6, 12>(value)));

      // Shift 26
      value = value_initial;
      value &= ~(value8mask << 26);
      value |= (value8 << 26);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t, 6, 26>(value)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t, 6, 26>(value)));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_sign_extend_template1b_constexpr)
    {
      char temp[etl::sign_extend<int8_t, 6, 0>(0x1A)];

      CHECK_EQUAL((etl::sign_extend<int8_t, 6, 0>(0x1A)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_sign_extend_template2)
    {
      uint8_t value8 = 0x2A;
      value8 &= 0x3FU; // 6 bit number.

      CHECK_EQUAL(-22, (etl::sign_extend<int8_t>(value8, 6)));
      CHECK_EQUAL(-22, (etl::sign_extend<int16_t>(value8, 6)));
      CHECK_EQUAL(-22, (etl::sign_extend<int32_t>(value8, 6)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t>(value8, 6)));

      uint16_t value16 = 0x2AAAU;
      value16 &= 0x3FFFU; // 14 bit number.

      CHECK_EQUAL(-5462, (etl::sign_extend<int16_t>(value16, 14)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int32_t>(value16, 14)));
      CHECK_EQUAL(-5462, (etl::sign_extend<int64_t>(value16, 14)));

      uint32_t value32 = 0x2AAAAAAAUL;
      value32 &= 0x3FFFFFFFUL; // 30 bit number.

      CHECK_EQUAL(-357913942, (etl::sign_extend<int32_t>(value32, 30)));
      CHECK_EQUAL(-357913942, (etl::sign_extend<int64_t>(value32, 30)));

      value8 = 0x0AU;
      value8 &= 0x3FU; // 6 bit number.

      CHECK_EQUAL(10, (etl::sign_extend<int8_t>(value8, 6)));
      CHECK_EQUAL(10, (etl::sign_extend<int16_t>(value8, 6)));
      CHECK_EQUAL(10, (etl::sign_extend<int32_t>(value8, 6)));
      CHECK_EQUAL(10, (etl::sign_extend<int64_t>(value8, 6)));

      value16 = 0x0AAAU;
      value16 &= 0x3FFFU; // 14 bit number.

      CHECK_EQUAL(2730, (etl::sign_extend<int16_t>(value16, 14)));
      CHECK_EQUAL(2730, (etl::sign_extend<int32_t>(value16, 14)));
      CHECK_EQUAL(2730, (etl::sign_extend<int64_t>(value16, 14)));

      value32 = 0x0AAAAAAAUL;
      value32 &= 0x3FFFFFFFUL; // 30 bit number.

      CHECK_EQUAL(178956970, (etl::sign_extend<int32_t>(value32, 30)));
      CHECK_EQUAL(178956970, (etl::sign_extend<int64_t>(value32, 30)));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_sign_extend_template2_constexpr)
    {
      char temp[etl::sign_extend<int8_t>(0x1AU, 6)];

      CHECK_EQUAL((etl::sign_extend<int8_t>(0x1AU, 6)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_sign_extend_template2b)
    {
      uint8_t value8 = 0x2AU;
      uint8_t value8mask = 0x3FU;

      const uint32_t value_initial = 0x55555555UL;

      uint32_t value;

      // Shift 0
      value = value_initial;
      value &= ~value8mask;
      value |= value8;

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t>(value, 6, 0)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t>(value, 6, 0)));

      // Shift 3
      value = value_initial;
      value &= ~(value8mask << 3);
      value |= (value8 << 3);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t>(value, 6, 3)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t>(value, 6, 3)));

      // Shift 6
      value = value_initial;
      value &= ~(value8mask << 6);
      value |= (value8 << 6);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t>(value, 6, 6)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t>(value, 6, 6)));

      // Shift 12
      value = value_initial;
      value &= ~(value8mask << 12);
      value |= (value8 << 12);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t>(value, 6, 12)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t>(value, 6, 12)));

      // Shift 26
      value = value_initial;
      value &= ~(value8mask << 26);
      value |= (value8 << 26);

      CHECK_EQUAL(-22, (etl::sign_extend<int32_t>(value, 6, 26)));
      CHECK_EQUAL(-22, (etl::sign_extend<int64_t>(value, 6, 26)));
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_sign_extend_template2b_constexpr)
    {
      char temp[etl::sign_extend<int8_t>(0x1AU, 6, 0)];

      CHECK_EQUAL((etl::sign_extend<int8_t>(0x1AU, 6, 0)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_bit)
    {
      const uint32_t N = 1UL;

      CHECK_EQUAL(N <<  0U, etl::bit<0>::value);
      CHECK_EQUAL(N <<  1U, etl::bit<1>::value);
      CHECK_EQUAL(N <<  2U, etl::bit<2>::value);
      CHECK_EQUAL(N <<  3U, etl::bit<3>::value);
      CHECK_EQUAL(N <<  4U, etl::bit<4>::value);
      CHECK_EQUAL(N <<  5U, etl::bit<5>::value);
      CHECK_EQUAL(N <<  6U, etl::bit<6>::value);
      CHECK_EQUAL(N <<  7U, etl::bit<7>::value);
      CHECK_EQUAL(N <<  8U, etl::bit<8>::value);
      CHECK_EQUAL(N <<  9U, etl::bit<9>::value);
      CHECK_EQUAL(N << 10U, etl::bit<10>::value);
      CHECK_EQUAL(N << 11U, etl::bit<11>::value);
      CHECK_EQUAL(N << 12U, etl::bit<12>::value);
      CHECK_EQUAL(N << 13U, etl::bit<13>::value);
      CHECK_EQUAL(N << 14U, etl::bit<14>::value);
      CHECK_EQUAL(N << 15U, etl::bit<15>::value);
      CHECK_EQUAL(N << 16U, etl::bit<16>::value);
      CHECK_EQUAL(N << 17U, etl::bit<17>::value);
      CHECK_EQUAL(N << 18U, etl::bit<18>::value);
      CHECK_EQUAL(N << 19U, etl::bit<19>::value);
      CHECK_EQUAL(N << 20U, etl::bit<20>::value);
      CHECK_EQUAL(N << 21U, etl::bit<21>::value);
      CHECK_EQUAL(N << 22U, etl::bit<22>::value);
      CHECK_EQUAL(N << 23U, etl::bit<23>::value);
      CHECK_EQUAL(N << 24U, etl::bit<24>::value);
      CHECK_EQUAL(N << 25U, etl::bit<25>::value);
      CHECK_EQUAL(N << 26U, etl::bit<26>::value);
      CHECK_EQUAL(N << 27U, etl::bit<27>::value);
      CHECK_EQUAL(N << 28U, etl::bit<28>::value);
      CHECK_EQUAL(N << 29U, etl::bit<29>::value);
      CHECK_EQUAL(N << 30U, etl::bit<30>::value);
      CHECK_EQUAL(N << 31U, etl::bit<31>::value);
    }

    //*************************************************************************
    TEST(test_bits_constants)
    {
      const uint32_t N = 1UL;

      CHECK_EQUAL(N <<  0U, etl::b0);
      CHECK_EQUAL(N <<  1U, etl::b1);
      CHECK_EQUAL(N <<  2U, etl::b2);
      CHECK_EQUAL(N <<  3U, etl::b3);
      CHECK_EQUAL(N <<  4U, etl::b4);
      CHECK_EQUAL(N <<  5U, etl::b5);
      CHECK_EQUAL(N <<  6U, etl::b6);
      CHECK_EQUAL(N <<  7U, etl::b7);
      CHECK_EQUAL(N <<  8U, etl::b8);
      CHECK_EQUAL(N <<  9U, etl::b9);
      CHECK_EQUAL(N << 10U, etl::b10);
      CHECK_EQUAL(N << 11U, etl::b11);
      CHECK_EQUAL(N << 12U, etl::b12);
      CHECK_EQUAL(N << 13U, etl::b13);
      CHECK_EQUAL(N << 14U, etl::b14);
      CHECK_EQUAL(N << 15U, etl::b15);
      CHECK_EQUAL(N << 16U, etl::b16);
      CHECK_EQUAL(N << 17U, etl::b17);
      CHECK_EQUAL(N << 18U, etl::b18);
      CHECK_EQUAL(N << 19U, etl::b19);
      CHECK_EQUAL(N << 20U, etl::b20);
      CHECK_EQUAL(N << 21U, etl::b21);
      CHECK_EQUAL(N << 22U, etl::b22);
      CHECK_EQUAL(N << 23U, etl::b23);
      CHECK_EQUAL(N << 24U, etl::b24);
      CHECK_EQUAL(N << 25U, etl::b25);
      CHECK_EQUAL(N << 26U, etl::b26);
      CHECK_EQUAL(N << 27U, etl::b27);
      CHECK_EQUAL(N << 28U, etl::b28);
      CHECK_EQUAL(N << 29U, etl::b29);
      CHECK_EQUAL(N << 30U, etl::b30);
      CHECK_EQUAL(N << 31U, uint32_t(etl::b31));
    }

    //*************************************************************************
    TEST(test_binary_fill)
    {
      CHECK_EQUAL(0x12U, (etl::binary_fill<uint8_t>(uint8_t(0x12U))));
      CHECK_EQUAL(0x1212U, (etl::binary_fill<uint16_t>(uint8_t(0x12U))));
      CHECK_EQUAL(0x12121212UL, (etl::binary_fill<uint32_t>(uint8_t(0x12U))));
      CHECK_EQUAL(0x1212121212121212ULL, (etl::binary_fill<uint64_t>(uint8_t(0x12U))));

      CHECK_EQUAL(0x12, (etl::binary_fill<int8_t>(int8_t(0x12))));
      CHECK_EQUAL(0x1212, (etl::binary_fill<int16_t>(int8_t(0x12))));
      CHECK_EQUAL(0x12121212L, (etl::binary_fill<int32_t>(int8_t(0x12))));
      CHECK_EQUAL(0x1212121212121212LL, (etl::binary_fill<int64_t>(int8_t(0x12))));

      CHECK_EQUAL(0x1234U, (etl::binary_fill<uint16_t>(uint16_t(0x1234U))));
      CHECK_EQUAL(0x12341234UL, (etl::binary_fill<uint32_t>(uint16_t(0x1234U))));
      CHECK_EQUAL(0x1234123412341234ULL, (etl::binary_fill<uint64_t>(uint16_t(0x1234U))));

      CHECK_EQUAL(0x1234, (etl::binary_fill<int16_t>(int16_t(0x1234))));
      CHECK_EQUAL(0x12341234L, (etl::binary_fill<int32_t>(int16_t(0x1234))));
      CHECK_EQUAL(0x1234123412341234LL, (etl::binary_fill<int64_t>(int16_t(0x1234))));

      CHECK_EQUAL(0x12345678UL, (etl::binary_fill<uint32_t>(uint32_t(0x12345678UL))));
      CHECK_EQUAL(0x1234567812345678ULL, (etl::binary_fill<uint64_t>(uint32_t(0x12345678UL))));

      CHECK_EQUAL(int32_t(0x12345678L), int32_t(etl::binary_fill<uint32_t>(int32_t(0x12345678L))));
      CHECK_EQUAL(int64_t(0x1234567812345678LL), int64_t(etl::binary_fill<int64_t>(uint32_t(0x12345678UL))));

      CHECK_EQUAL(0x123456789ABCDEF0ULL, (etl::binary_fill<uint64_t>(uint64_t(0x123456789ABCDEF0ULL))));

      CHECK_EQUAL(int64_t(0x123456789ABCDEF0LL), int64_t(etl::binary_fill<int64_t>(int64_t(0x123456789ABCDEF0LL))));
    }

    //*************************************************************************
    TEST(test_binary_fill_partial_compile_time)
    {
      CHECK_EQUAL(0x12U, (etl::binary_fill<uint8_t, uint8_t, 0x12U>()));
      CHECK_EQUAL(0x1212U, (etl::binary_fill<uint16_t, uint8_t, 0x12U>()));
      CHECK_EQUAL(0x12121212UL, (etl::binary_fill<uint32_t, uint8_t, 0x12U>()));
      CHECK_EQUAL(0x1212121212121212ULL, (etl::binary_fill<uint64_t, uint8_t, 0x12U>()));

      CHECK_EQUAL(0x12U, (etl::binary_fill<int8_t, int8_t, 0x12>()));
      CHECK_EQUAL(0x1212U, (etl::binary_fill<int16_t, int8_t, 0x12>()));
      CHECK_EQUAL(0x12121212UL, (etl::binary_fill<int32_t, int8_t, 0x12>()));
      CHECK_EQUAL(0x1212121212121212ULL, (etl::binary_fill<int64_t, int8_t, 0x12>()));

      CHECK_EQUAL(0x1234U, (etl::binary_fill<uint16_t, uint16_t, 0x1234U>()));
      CHECK_EQUAL(0x12341234UL, (etl::binary_fill<uint32_t, uint16_t, 0x1234U>()));
      CHECK_EQUAL(0x1234123412341234ULL, (etl::binary_fill<uint64_t, uint16_t, 0x1234U>()));

      CHECK_EQUAL(0x1234U, (etl::binary_fill<int16_t, int16_t, 0x1234>()));
      CHECK_EQUAL(0x12341234UL, (etl::binary_fill<int32_t, int16_t, 0x1234>()));
      CHECK_EQUAL(0x1234123412341234ULL, (etl::binary_fill<int64_t, int16_t, 0x1234>()));

      CHECK_EQUAL(0x12345678UL, (etl::binary_fill<uint32_t, uint32_t, 0x12345678UL>()));
      CHECK_EQUAL(0x1234567812345678ULL, (etl::binary_fill<uint64_t, uint32_t, 0x12345678UL>()));

      CHECK_EQUAL(0x12345678UL, (etl::binary_fill<int32_t, int32_t, 0x12345678L>()));
      CHECK_EQUAL(0x1234567812345678ULL, (etl::binary_fill<int64_t, int32_t, 0x12345678L>()));

      CHECK_EQUAL(0x123456789ABCDEF0ULL, (etl::binary_fill<uint64_t, uint64_t, 0x123456789ABCDEF0ULL>()));

      CHECK_EQUAL(int64_t(0x123456789ABCDEF0LL), int64_t(etl::binary_fill<int64_t, int64_t, 0x123456789ABCDEF0LL>()));
    }

    //*************************************************************************
    TEST(test_has_zero_byte)
    {
      CHECK(!(etl::has_zero_byte(uint8_t(0x01U))));
      CHECK((etl::has_zero_byte(uint8_t(0x00U))));

      CHECK(!(etl::has_zero_byte(int8_t(0x01))));
      CHECK((etl::has_zero_byte(int8_t(0x00U))));

      CHECK(!(etl::has_zero_byte(uint16_t(0x0123U))));
      CHECK((etl::has_zero_byte(uint16_t(0x0100U))));

      CHECK(!(etl::has_zero_byte(int16_t(0x0123))));
      CHECK((etl::has_zero_byte(int16_t(0x0100))));

      CHECK(!(etl::has_zero_byte(uint32_t(0x01234567UL))));
      CHECK((etl::has_zero_byte(uint32_t(0x01230067UL))));

      CHECK(!(etl::has_zero_byte(int32_t(0x01234567L))));
      CHECK((etl::has_zero_byte(int32_t(0x01230067L))));

      CHECK(!(etl::has_zero_byte(uint64_t(0x0123456789ABCDEFULL))));
      CHECK((etl::has_zero_byte(uint64_t(0x012345678900CDEFULL))));

      CHECK(!(etl::has_zero_byte(int64_t(0x0123456789ABCDEFLL))));
      CHECK((etl::has_zero_byte(int64_t(0x012345678900CDEFLL))));
    }

    //*************************************************************************
    TEST(test_has_zero_byte_partial_compile_time)
    {
      CHECK(!(etl::has_zero_byte<uint8_t, 0x01U>()));
      CHECK((etl::has_zero_byte<uint8_t, 0x00U>()));

      CHECK(!(etl::has_zero_byte<int8_t, 0x01>()));
      CHECK((etl::has_zero_byte<int8_t, 0x00>()));

      CHECK(!(etl::has_zero_byte<uint16_t, 0x0123U>()));
      CHECK((etl::has_zero_byte<uint16_t, 0x0100U>()));

      CHECK(!(etl::has_zero_byte<int16_t, 0x0123>()));
      CHECK((etl::has_zero_byte<int16_t, 0x0100>()));

      CHECK(!(etl::has_zero_byte<uint32_t, 0x01234567UL>()));
      CHECK((etl::has_zero_byte<uint32_t, 0x01230067UL>()));

      CHECK(!(etl::has_zero_byte<int32_t, 0x01234567L>()));
      CHECK((etl::has_zero_byte<int32_t, 0x01230067L>()));

      CHECK(!(etl::has_zero_byte<uint64_t, 0x0123456789ABCDEFULL>()));
      CHECK((etl::has_zero_byte<uint64_t, 0x012345678900CDEFULL>()));

      CHECK(!(etl::has_zero_byte<int64_t, 0x0123456789ABCDEFLL>()));
      CHECK((etl::has_zero_byte<int64_t, 0x012345678900CDEFLL>()));
    }

    //*************************************************************************
    TEST(test_has_byte_n_run_time)
    {
      CHECK(!(etl::has_byte_n(uint8_t(0x01U), 0x12U)));
      CHECK((etl::has_byte_n(uint8_t(0x01U), 0x01U)));

      CHECK(!(etl::has_byte_n(uint8_t(0x01U), 0x12U)));
      CHECK((etl::has_byte_n(uint8_t(0x01U), 0x01U)));

      CHECK(!(etl::has_byte_n(uint16_t(0x0123U), 0x12U)));
      CHECK((etl::has_byte_n(uint16_t(0x0123U), 0x23U)));

      CHECK(!(etl::has_byte_n(int16_t(0x0123), 0x12)));
      CHECK((etl::has_byte_n(int16_t(0x0123), 0x23)));

      CHECK(!(etl::has_byte_n(uint32_t(0x01234567UL), 0x12U)));
      CHECK((etl::has_byte_n(uint32_t(0x01234567UL), 0x45U)));

      CHECK(!(etl::has_byte_n(int32_t(0x01234567L), 0x12)));
      CHECK((etl::has_byte_n(int32_t(0x01234567L), 0x45)));

      CHECK(!(etl::has_byte_n(uint64_t(0x0123456789ABCDEFULL), 0x12U)));
      CHECK((etl::has_byte_n(uint64_t(0x0123456789ABCDEFULL), 0xABU)));

      CHECK(!(etl::has_byte_n(int64_t(0x0123456789ABCDEFLL), 0x12)));
      CHECK((etl::has_byte_n(int64_t(0x0123456789ABCDEFLL), 0xAB)));
    }

    //*************************************************************************
    TEST(test_has_byte_n_partial_run_time)
    {
      CHECK(!(etl::has_byte_n<uint8_t, 0x12U>(uint8_t(0x01U))));
      CHECK((etl::has_byte_n<uint8_t, 0x01U>(uint8_t(0x01U))));

      CHECK(!(etl::has_byte_n<int8_t, 0x12>(int8_t(0x01))));
      CHECK((etl::has_byte_n<int8_t, 0x01>(int8_t(0x01))));

      CHECK(!(etl::has_byte_n<uint16_t, 0x12U>(uint16_t(0x0123))));
      CHECK((etl::has_byte_n<uint16_t, 0x23U>(uint16_t(0x0123))));

      CHECK(!(etl::has_byte_n<int16_t, 0x12>(int16_t(0x0123))));
      CHECK((etl::has_byte_n<int16_t, 0x23>(int16_t(0x0123))));

      CHECK(!(etl::has_byte_n<uint32_t, 0x12U>(uint32_t(0x01234567UL))));
      CHECK((etl::has_byte_n<uint32_t, 0x45U>(uint32_t(0x01234567UL))));

      CHECK(!(etl::has_byte_n<int32_t, 0x12>(int32_t(0x01234567L))));
      CHECK((etl::has_byte_n<int32_t, 0x45>(int32_t(0x01234567L))));

      CHECK(!(etl::has_byte_n<uint64_t, 0x12U>(uint64_t(0x0123456789ABCDEFULL))));
      CHECK((etl::has_byte_n<uint64_t, 0xABU>(uint64_t(0x0123456789ABCDEFULL))));

      CHECK(!(etl::has_byte_n<int64_t, 0x12>(int64_t(0x0123456789ABCDEFLL))));
      CHECK((etl::has_byte_n<int64_t, 0xAB>(int64_t(0x0123456789ABCDEFLL))));
    }

	  //*************************************************************************
	  TEST(test_binary_merge)
	  {
		  CHECK_EQUAL((etl::binary_merge(uint8_t(0x12U), uint8_t(0x34U), uint8_t(0xF0U))), uint8_t(0x14U));
		  CHECK_EQUAL((etl::binary_merge<uint8_t, 0xF0U>(uint8_t(0x12U), uint8_t(0x34U))), uint8_t(0x14U));

		  CHECK_EQUAL((etl::binary_merge(uint16_t(0x1234U), uint16_t(0x3456U), uint16_t(0xF0F0U))), uint16_t(0x1436U));
		  CHECK_EQUAL((etl::binary_merge<uint16_t, 0xF0F0U>(uint16_t(0x1234U), uint16_t(0x3456U))), uint16_t(0x1436U));

		  CHECK_EQUAL((etl::binary_merge(uint32_t(0x12345678UL), uint32_t(0x3456789AUL), uint32_t(0xF0F0F0F0UL))), uint32_t(0x1436587AUL));
		  CHECK_EQUAL((etl::binary_merge<uint32_t, 0xF0F0F0F0UL>(uint32_t(0x12345678UL), uint32_t(0x3456789AUL))), uint32_t(0x1436587AUL));

		  CHECK_EQUAL((etl::binary_merge(uint64_t(0x123456789ABCDEF0ULL), uint64_t(0x3456789ABCDEF012ULL), uint64_t(0xF0F0F0F0F0F0F0F0ULL))), uint64_t(0x1436587A9CBED0F2ULL));
		  CHECK_EQUAL((etl::binary_merge<uint64_t, 0xF0F0F0F0F0F0F0F0ULL>(uint64_t(0x123456789ABCDEF0ULL), uint64_t(0x3456789ABCDEF012ULL))), uint64_t(0x1436587A9CBED0F2ULL));
	  }

	  //*************************************************************************
	  TEST(test_binary_interleave)
	  {
		  CHECK_EQUAL((etl::binary_interleave(uint8_t(0x5AU), uint8_t(0xA5U))), uint16_t(0x9966U));
		  CHECK_EQUAL((etl::binary_interleave(uint16_t(0x5A5AU), uint16_t(0xA5A5U))), uint32_t(0x99669966UL));
		  CHECK_EQUAL((etl::binary_interleave(uint32_t(0x5A5A5A5AUL), uint32_t(0xA5A5A5A5UL))), uint64_t(0x9966996699669966ULL));
	  }

	  //*************************************************************************
	  TEST(test_is_odd)
	  {
		  CHECK(etl::is_odd(1));
		  CHECK(!etl::is_odd(2));
	  }

	  //*************************************************************************
	  TEST(test_is_even)
	  {
		  CHECK(!etl::is_even(1));
		  CHECK(etl::is_even(2));
	  }

    //*************************************************************************
    TEST(test_count_trailing_zeros_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_trailing_zeros(value)), int(etl::count_trailing_zeros(value)));

        if (test_trailing_zeros(value) != etl::count_trailing_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_8_constexpr)
    {
      char temp[etl::count_trailing_zeros(uint8_t(0x08))];

      CHECK_EQUAL(test_trailing_zeros(uint8_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_zeros_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(test_trailing_zeros(value), etl::count_trailing_zeros(value));

        if (test_trailing_zeros(value) != etl::count_trailing_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_16_constexpr)
    {
      char temp[etl::count_trailing_zeros(uint16_t(0x08))];

      CHECK_EQUAL(test_trailing_zeros(uint16_t(0x08)), sizeof(temp));
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

        CHECK_EQUAL(test_trailing_zeros(value), etl::count_trailing_zeros(value));

        if (test_trailing_zeros(value) != etl::count_trailing_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_32_constexpr)
    {
      char temp[etl::count_trailing_zeros(uint32_t(0x08))];

      CHECK_EQUAL(test_trailing_zeros(uint32_t(0x08)), sizeof(temp));
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

        CHECK_EQUAL(test_trailing_zeros(value), etl::count_trailing_zeros(value));

        if (test_trailing_zeros(value) != etl::count_trailing_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_zeros_64_constexpr)
    {
      char temp[etl::count_trailing_zeros(uint64_t(0x08))];

      CHECK_EQUAL(etl::count_trailing_zeros(uint64_t(0x08)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_ones_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_trailing_ones(value)), int(etl::count_trailing_ones(value)));

        if (test_trailing_ones(value) != etl::count_trailing_ones(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_ones_8_constexpr)
    {
      char temp[etl::count_trailing_ones(uint8_t(0x0F))];

      CHECK_EQUAL(test_trailing_ones(uint8_t(0x0F)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_ones_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(int(test_trailing_ones(value)), int(etl::count_trailing_ones(value)));

        if (test_trailing_ones(value) != etl::count_trailing_ones(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_ones_16_constexpr)
    {
      char temp[etl::count_trailing_ones(uint16_t(0x000F))];

      CHECK_EQUAL(test_trailing_ones(uint16_t(0x000F)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_ones_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(test_trailing_ones(value), etl::count_trailing_ones(value));

        if (test_trailing_ones(value) != etl::count_trailing_ones(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_ones_32_constexpr)
    {
      char temp[etl::count_trailing_ones(uint32_t(0x0000000F))];

      CHECK_EQUAL(test_trailing_ones(uint32_t(0x0000000F)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_trailing_ones_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0UL; i < 100000UL; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(test_trailing_ones(value), etl::count_trailing_ones(value));

        if (test_trailing_ones(value) != etl::count_trailing_ones(value))
        {
          break;
        }
      }
    }
#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_trailing_ones_64_constexpr)
    {
      char temp[etl::count_trailing_ones(uint64_t(0x000000000000000F))];

      CHECK_EQUAL(test_trailing_ones(uint64_t(0x000000000000000F)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_zeros_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::count_leading_zeros(value)));

        if (test_leading_zeros(value) != etl::count_leading_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_zeros_8_constexpr)
    {
      char temp[etl::count_leading_zeros(uint8_t(0x01U))];
      
      CHECK_EQUAL(test_leading_zeros(uint8_t(0x01U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_zeros_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::count_leading_zeros(value)));

        if (test_leading_zeros(value) != etl::count_leading_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_zeros_16_constexpr)
    {
      char temp[etl::count_leading_zeros(uint16_t(0x0800U))];

      CHECK_EQUAL(test_leading_zeros(uint16_t(0x0800U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_zeros_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::count_leading_zeros(value)));

        if (test_leading_zeros(value) != etl::count_leading_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_zeros_32_constexpr)
    {
      char temp[etl::count_leading_zeros(uint32_t(0x08000000U))];

      CHECK_EQUAL(test_leading_zeros(uint32_t(0x08000000U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_zeros_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(int(test_leading_zeros(value)), int(etl::count_leading_zeros(value)));

        if (test_leading_zeros(value) != etl::count_leading_zeros(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_zeros_64_constexpr)
    {
      char temp[etl::count_leading_zeros(uint64_t(0x0800000000000000U))];

      CHECK_EQUAL(test_leading_zeros(uint64_t(0x0800000000000000U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_ones_8)
    {
      for (size_t i = 0; i < 256; ++i)
      {
        uint8_t value = uint8_t(i);

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::count_leading_ones(value)));

        if (test_leading_ones(value) != etl::count_leading_ones(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_ones_8_constexpr)
    {
      char temp[etl::count_leading_ones(uint8_t(0xF0U))];

      CHECK_EQUAL(test_leading_ones(uint8_t(0xF0U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_ones_16)
    {
      for (size_t i = 0; i < 65536; ++i)
      {
        uint16_t value = uint16_t(i);

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::count_leading_ones(value)));

        if (test_leading_ones(value) != etl::count_leading_ones(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_ones_16_constexpr)
    {
      char temp[etl::count_leading_ones(uint16_t(0xF000U))];

      CHECK_EQUAL(test_leading_ones(uint16_t(0xF000U)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_ones_32)
    {
      etl::fnv_1a_32 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint32_t value = hash.value();

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::count_leading_ones(value)));

        if (test_leading_ones(value) != etl::count_leading_ones(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_ones_32_constexpr)
    {
      char temp[etl::count_leading_ones(uint32_t(0xF0000000UL))];

      CHECK_EQUAL(test_leading_ones(uint32_t(0xF0000000UL)), sizeof(temp));
    }
#endif

    //*************************************************************************
    TEST(test_count_leading_ones_64)
    {
      etl::fnv_1a_64 hash;

      for (size_t i = 0; i < 100000; ++i)
      {
        hash.add(1);

        uint64_t value = hash.value();

        CHECK_EQUAL(int(test_leading_ones(value)), int(etl::count_leading_ones(value)));

        if (test_leading_ones(value) != etl::count_leading_ones(value))
        {
          break;
        }
      }
    }

#if !defined(ETL_FORCE_NO_ADVANCED_CPP)
    //*************************************************************************
    TEST(test_count_leading_ones_64_constexpr)
    {
      char temp[etl::count_leading_ones(uint64_t(0xF000000000000000UL))];

      CHECK_EQUAL(test_leading_ones(uint64_t(0xF000000000000000UL)), sizeof(temp));
    }
#endif
  };
}

